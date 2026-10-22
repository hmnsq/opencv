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

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ "opencv2/core.hpp"
#அந்தர்பாவ "opencv2/core/ocl.hpp"
#அந்தர்பாவ "opencv2/core/directx.hpp"
#அந்தர்பாவ "opencl_kernels_core.hpp"

#ifdef HAVE_DIRECTX
#அந்தர்பாவ <vector>
#அந்தர்பாவ "directx.inc.hpp"
#இல்லையெனில் // HAVE_DIRECTX
#define NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#endif

#ifndef HAVE_OPENCL
#define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

using நாமாகாஶ cv::ocl;

நாமாகாஶ cv { நாமாகாஶ directx {

முழு getTypeFromDXGI_FORMAT(மாறிலி முழு iDXGI_FORMAT)
{
    CV_UNUSED(iDXGI_FORMAT);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#இல்லையெனில்
    மாறிலி முழு errorType = -1;
    தேர்வு ((எண்ணிக்கை DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    நிலை DXGI_FORMAT_R32G32B32A32_FLOAT: திரும்பு CV_32FC4;
    நிலை DXGI_FORMAT_R32G32B32A32_UINT:
    நிலை DXGI_FORMAT_R32G32B32A32_SINT:  திரும்பு CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    நிலை DXGI_FORMAT_R32G32B32_FLOAT: திரும்பு CV_32FC3;
    நிலை DXGI_FORMAT_R32G32B32_UINT:
    நிலை DXGI_FORMAT_R32G32B32_SINT: திரும்பு CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    நிலை DXGI_FORMAT_R16G16B16A16_FLOAT: திரும்பு CV_16FC4;
    நிலை DXGI_FORMAT_R16G16B16A16_UNORM:
    நிலை DXGI_FORMAT_R16G16B16A16_UINT: திரும்பு CV_16UC4;
    நிலை DXGI_FORMAT_R16G16B16A16_SNORM:
    நிலை DXGI_FORMAT_R16G16B16A16_SINT: திரும்பு CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    நிலை DXGI_FORMAT_R32G32_FLOAT: திரும்பு CV_32FC2;
    நிலை DXGI_FORMAT_R32G32_UINT:
    நிலை DXGI_FORMAT_R32G32_SINT: திரும்பு CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    நிலை DXGI_FORMAT_R8G8B8A8_UNORM:
    நிலை DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    நிலை DXGI_FORMAT_R8G8B8A8_UINT: திரும்பு CV_8UC4;
    நிலை DXGI_FORMAT_R8G8B8A8_SNORM:
    நிலை DXGI_FORMAT_R8G8B8A8_SINT: திரும்பு CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    நிலை DXGI_FORMAT_R16G16_FLOAT: திரும்பு CV_16FC2;
    நிலை DXGI_FORMAT_R16G16_UNORM:
    நிலை DXGI_FORMAT_R16G16_UINT: திரும்பு CV_16UC2;
    நிலை DXGI_FORMAT_R16G16_SNORM:
    நிலை DXGI_FORMAT_R16G16_SINT: திரும்பு CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    நிலை DXGI_FORMAT_D32_FLOAT:
    நிலை DXGI_FORMAT_R32_FLOAT: திரும்பு CV_32FC1;
    நிலை DXGI_FORMAT_R32_UINT:
    நிலை DXGI_FORMAT_R32_SINT: திரும்பு CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    நிலை DXGI_FORMAT_R8G8_UNORM:
    நிலை DXGI_FORMAT_R8G8_UINT: திரும்பு CV_8UC2;
    நிலை DXGI_FORMAT_R8G8_SNORM:
    நிலை DXGI_FORMAT_R8G8_SINT: திரும்பு CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    நிலை DXGI_FORMAT_R16_FLOAT: திரும்பு CV_16FC1;
    நிலை DXGI_FORMAT_D16_UNORM:
    நிலை DXGI_FORMAT_R16_UNORM:
    நிலை DXGI_FORMAT_R16_UINT: திரும்பு CV_16UC1;
    நிலை DXGI_FORMAT_R16_SNORM:
    நிலை DXGI_FORMAT_R16_SINT: திரும்பு CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    நிலை DXGI_FORMAT_R8_UNORM:
    நிலை DXGI_FORMAT_R8_UINT: திரும்பு CV_8UC1;
    நிலை DXGI_FORMAT_R8_SNORM:
    நிலை DXGI_FORMAT_R8_SINT: திரும்பு CV_8SC1;
    நிலை DXGI_FORMAT_A8_UNORM: திரும்பு CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    நிலை DXGI_FORMAT_R8G8_B8G8_UNORM:
    நிலை DXGI_FORMAT_G8R8_G8B8_UNORM: திரும்பு CV_8UC4;
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
    நிலை DXGI_FORMAT_B8G8R8A8_UNORM:
    நிலை DXGI_FORMAT_B8G8R8X8_UNORM: திரும்பு CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    நிலை DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: திரும்பு CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    நிலை DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: திரும்பு CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#ifdef HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    நிலை DXGI_FORMAT_NV12: திரும்பு CV_8UC3;
#endif
    இயல்பு: நிறுத்து;
    }
    திரும்பு errorType;
#endif
}

முழு getTypeFromD3DFORMAT(மாறிலி முழு iD3DFORMAT)
{
    CV_UNUSED(iD3DFORMAT);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#இல்லையெனில்
    மாறிலி முழு errorType = -1;
    தேர்வு ((எண்ணிக்கை _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    நிலை D3DFMT_R8G8B8: திரும்பு CV_8UC3;
    நிலை D3DFMT_A8R8G8B8:
    நிலை D3DFMT_X8R8G8B8: திரும்பு CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    நிலை D3DFMT_A8: திரும்பு CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    நிலை D3DFMT_A8B8G8R8:
    நிலை D3DFMT_X8B8G8R8: திரும்பு CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    நிலை D3DFMT_A8P8: திரும்பு CV_8UC2;
    நிலை D3DFMT_P8: திரும்பு CV_8UC1;

    நிலை D3DFMT_L8: திரும்பு CV_8UC1;
    நிலை D3DFMT_A8L8: திரும்பு CV_8UC2;
    //case D3DFMT_A4L4:

    நிலை D3DFMT_V8U8: திரும்பு CV_8UC2;
    //case D3DFMT_L6V5U5:
    நிலை D3DFMT_X8L8V8U8:
    நிலை D3DFMT_Q8W8V8U8: திரும்பு CV_8UC4;
    நிலை D3DFMT_V16U16: திரும்பு CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    நிலை D3DFMT_D16_LOCKABLE: திரும்பு CV_16UC1;
    நிலை D3DFMT_D32: திரும்பு CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    நிலை D3DFMT_D16: திரும்பு CV_16UC1;

    நிலை D3DFMT_D32F_LOCKABLE: திரும்பு CV_32FC1;
    இயல்பு: நிறுத்து;
    }
    திரும்பு errorType;
#endif
}

#எனில் defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#ifdef HAVE_OPENCL_D3D11_NV
வகுப்பு OpenCL_D3D11_NV : பொது ocl::Context::UserContext
{
பொது:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D11Texture2DNV");
        clEnqueueAcquireD3D11ObjectsNV = (clEnqueueAcquireD3D11ObjectsNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        clEnqueueReleaseD3D11ObjectsNV = (clEnqueueReleaseD3D11ObjectsNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        எனில் (!clCreateFromD3D11Texture2DNV || !clEnqueueAcquireD3D11ObjectsNV || !clEnqueueReleaseD3D11ObjectsNV)
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
#endif

வகுப்பு OpenCL_D3D11 : பொது ocl::Context::UserContext
{
பொது:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D11Texture2DKHR");
        clEnqueueAcquireD3D11ObjectsKHR = (clEnqueueAcquireD3D11ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        clEnqueueReleaseD3D11ObjectsKHR = (clEnqueueReleaseD3D11ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        எனில் (!clCreateFromD3D11Texture2DKHR || !clEnqueueAcquireD3D11ObjectsKHR || !clEnqueueReleaseD3D11ObjectsKHR)
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

வகுப்பு OpenCL_D3D9 : பொது ocl::Context::UserContext
{
பொது:
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
        எனில் (!clCreateFromDX9MediaSurfaceKHR || !clEnqueueAcquireDX9MediaSurfacesKHR || !clEnqueueReleaseDX9MediaSurfacesKHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        எனில் (device)
            device->AddRef();
        எனில் (deviceEx)
            deviceEx->AddRef();
    }
    ~OpenCL_D3D9() {
        எனில் (device)
            device->Release();
        எனில் (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    clEnqueueAcquireDX9MediaSurfacesKHR_fn clEnqueueAcquireDX9MediaSurfacesKHR;
    clEnqueueReleaseDX9MediaSurfacesKHR_fn clEnqueueReleaseDX9MediaSurfacesKHR;
};

வகுப்பு OpenCL_D3D10 : பொது ocl::Context::UserContext
{
பொது:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D10Texture2DKHR");
        clEnqueueAcquireD3D10ObjectsKHR = (clEnqueueAcquireD3D10ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        clEnqueueReleaseD3D10ObjectsKHR = (clEnqueueReleaseD3D10ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        எனில் (!clCreateFromD3D10Texture2DKHR || !clEnqueueAcquireD3D10ObjectsKHR || !clEnqueueReleaseD3D10ObjectsKHR)
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
#endif

நாமாகாஶ ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_UNUSED(pD3D11Device);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#இல்லையெனில்
    cl_uint இலக்கம்_Platforms;
    cl_int status = clGetPlatformIDs(0, வெற்று, &இலக்கம்_Platforms);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    எனில் (இலக்கம்_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(இலக்கம்_Platforms);
    status = clGetPlatformIDs(இலக்கம்_Platforms, &platforms[0], வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    க்கு (முழு i = 0; i < (முழு)இலக்கம்_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        முழு found = -1;
        cl_device_id device = வெற்று;
        cl_uint இலக்கம்_Devices = 0;
        cl_context context = வெற்று;

#ifdef HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        எனில் (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            செய் {
                device = வெற்று;
                இலக்கம்_Devices = 0;
                status = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &இலக்கம்_Devices);
                எனில் (status != CL_SUCCESS)
                    நிறுத்து;
                எனில் (இலக்கம்_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                    எனில் (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    இல்லையெனில்
                    {
                        found = i;
                    }
                }
            } போது (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            எனில் (found < 0) செய் {
                device = வெற்று;
                இலக்கம்_Devices = 0;
                status = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &இலக்கம்_Devices);
                எனில் (status != CL_SUCCESS)
                    நிறுத்து;
                எனில் (இலக்கம்_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                    எனில் (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    இல்லையெனில்
                    {
                        found = i;
                    }
                }
            } போது (0);
            எனில் (found >= 0) {
                OpenCLExecutionContext clExecCtx;
                முயல்
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                பிடி (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    எறி;
                }
                clExecCtx.bind();
                திரும்பு const_cast<Context&>(clExecCtx.getContext());
            }
        }
#endif
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        எனில் (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            செய் {

                device = வெற்று;
                இலக்கம்_Devices = 0;

                status = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &இலக்கம்_Devices);

                எனில் (status != CL_SUCCESS)
                    நிறுத்து;
                எனில் (இலக்கம்_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            வெற்று, வெற்று
                    };
                    context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                    எனில் (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    இல்லையெனில்
                    {
                        found = i;
                    }
                }
            } போது (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            எனில் (found < 0) செய் {
                device = வெற்று;
                இலக்கம்_Devices = 0;
                status = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &இலக்கம்_Devices);
                எனில் (status != CL_SUCCESS)
                    நிறுத்து;
                எனில் (இலக்கம்_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            வெற்று, வெற்று
                    };
                    context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                    எனில் (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    இல்லையெனில்
                    {
                        found = i;
                    }
                }
            } போது (0);

            எனில் (found >= 0) {
                OpenCLExecutionContext clExecCtx;
                முயல்
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                பிடி (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    எறி;
                }
                clExecCtx.bind();
                திரும்பு const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_UNUSED(pD3D10Device);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#இல்லையெனில்
    cl_uint இலக்கம்_Platforms;
    cl_int status = clGetPlatformIDs(0, வெற்று, &இலக்கம்_Platforms);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    எனில் (இலக்கம்_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(இலக்கம்_Platforms);
    status = clGetPlatformIDs(இலக்கம்_Platforms, &platforms[0], வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    க்கு (முழு i = 0; i < (முழு)இலக்கம்_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        முழு found = -1;
        cl_device_id device = வெற்று;
        cl_uint இலக்கம்_Devices = 0;
        cl_context context = வெற்று;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        எனில் (!clGetDeviceIDsFromD3D10KHR)
            தொடர்;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        செய் {
            device = வெற்று;
            இலக்கம்_Devices = 0;
            status = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &இலக்கம்_Devices);
            எனில் (status != CL_SUCCESS)
                நிறுத்து;
            எனில் (இலக்கம்_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        வெற்று, வெற்று
                };
                context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                எனில் (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                இல்லையெனில்
                {
                    found = i;
                }
            }
        } போது (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        எனில் (found < 0) செய்
        {
            device = வெற்று;
            இலக்கம்_Devices = 0;
            status = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &இலக்கம்_Devices);
            எனில் (status != CL_SUCCESS)
                நிறுத்து;
            எனில் (இலக்கம்_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        வெற்று, வெற்று
                };
                context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                எனில் (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                இல்லையெனில்
                {
                    found = i;
                }
            }
        } போது (0);

        எனில் (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            முயல்
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            பிடி (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                எறி;
            }
            clExecCtx.bind();
            திரும்பு const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_UNUSED(pDirect3DDevice9Ex);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#இல்லையெனில்
    cl_uint இலக்கம்_Platforms;
    cl_int status = clGetPlatformIDs(0, வெற்று, &இலக்கம்_Platforms);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    எனில் (இலக்கம்_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(இலக்கம்_Platforms);
    status = clGetPlatformIDs(இலக்கம்_Platforms, &platforms[0], வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    க்கு (முழு i = 0; i < (முழு)இலக்கம்_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        முழு found = -1;
        cl_device_id device = வெற்று;
        cl_uint இலக்கம்_Devices = 0;
        cl_context context = வெற்று;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        எனில் (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            தொடர்;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        செய் {
            device = வெற்று;
            இலக்கம்_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &இலக்கம்_Devices);
            எனில் (status != CL_SUCCESS)
                நிறுத்து;
            எனில் (இலக்கம்_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        வெற்று, வெற்று
                };
                context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                எனில் (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                இல்லையெனில்
                {
                    found = i;
                }
            }
        } போது (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        எனில் (found < 0) செய்
        {
            device = வெற்று;
            இலக்கம்_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &இலக்கம்_Devices);
            எனில் (status != CL_SUCCESS)
                நிறுத்து;
            எனில் (இலக்கம்_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        வெற்று, வெற்று
                };
                context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                எனில் (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                இல்லையெனில்
                {
                    found = i;
                }
            }
        } போது (0);

        எனில் (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            முயல்
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D9>(platform, nullptr, pDirect3DDevice9Ex));
            }
            பிடி (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                எறி;
            }
            clExecCtx.bind();
            திரும்பு const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_UNUSED(pDirect3DDevice9);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#இல்லையெனில்
    cl_uint இலக்கம்_Platforms;
    cl_int status = clGetPlatformIDs(0, வெற்று, &இலக்கம்_Platforms);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    எனில் (இலக்கம்_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(இலக்கம்_Platforms);
    status = clGetPlatformIDs(இலக்கம்_Platforms, &platforms[0], வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    க்கு (முழு i = 0; i < (முழு)இலக்கம்_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        முழு found = -1;
        cl_device_id device = வெற்று;
        cl_uint இலக்கம்_Devices = 0;
        cl_context context = வெற்று;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        எனில் (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            தொடர்;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        செய் {
            device = வெற்று;
            இலக்கம்_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &இலக்கம்_Devices);
            எனில் (status != CL_SUCCESS)
                நிறுத்து;
            எனில் (இலக்கம்_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        வெற்று, வெற்று
                };
                context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                எனில் (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                இல்லையெனில்
                {
                    found = i;
                    நிறுத்து;
                }
            }
        } போது (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        எனில் (found < 0) செய்
        {
            device = வெற்று;
            இலக்கம்_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &இலக்கம்_Devices);
            எனில் (status != CL_SUCCESS)
                நிறுத்து;
            எனில் (இலக்கம்_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        வெற்று, வெற்று
                };
                context = clCreateContext(properties, 1, &device, வெற்று, வெற்று, &status);
                எனில் (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                இல்லையெனில்
                {
                    found = i;
                    நிறுத்து;
                }
            }
        } போது (0);

        எனில் (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            முயல்
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, nullptr));
            }
            பிடி (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                எறி;
            }
            clExecCtx.bind();
            திரும்பு const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

} // namespace cv::ocl

} // namespace directx


நாமாகாஶ ocl {

#எனில் defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#ifdef HAVE_DIRECTX_NV12

நிலையான
தருக்க ocl_convert_nv12_to_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem clBuffer,
    முழு step,
    முழு cols,
    முழு rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    எனில் (k.empty())
        திரும்பு பொய்;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    திரும்பு k.run(2, globalsize, 0, பொய்);
}


நிலையான
தருக்க ocl_convert_bgr_to_nv12(
    cl_mem clBuffer,
    முழு step,
    முழு cols,
    முழு rows,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    எனில் (k.empty())
        திரும்பு பொய்;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    திரும்பு k.run(2, globalsize, 0, பொய்);
}

#endif // HAVE_DIRECTX_NV12
#endif // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


நாமாகாஶ directx {

#எனில் defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
நிலையான வெறுமை __convertToD3D11Texture2DKHR(உள்ளீடு_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    முழு srcType = src.type();
    முழு textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (முழு)desc.Width && srcSize.height == (முழு)desc.Height);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;
#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#endif
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &status);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#endif

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        எனில்(!ocl::ocl_convert_bgr_to_nv12(clBuffer, (முழு)u.step[0], u.cols, u.rows, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    இல்லையெனில்
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, origin, region, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

#எனில் defined(HAVE_OPENCL_D3D11_NV)
நிலையான வெறுமை __convertToD3D11Texture2DNV(உள்ளீடு_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    முழு srcType = src.type();
    முழு textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (முழு)desc.Width && srcSize.height == (முழு)desc.Height);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;
#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#endif
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &status);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ifdef HAVE_DIRECTX_NV12
    எனில் (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#endif
    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        எனில்(!ocl::ocl_convert_bgr_to_nv12(clBuffer, (முழு)u.step[0], u.cols, u.rows, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    இல்லையெனில்
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, origin, region, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

#எனில் defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
நிலையான வெறுமை __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    முழு textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(Size(desc.Width, desc.Height), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &status);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#endif

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        எனில்(!ocl::ocl_convert_nv12_to_bgr(clImage, clImageUV, clBuffer, (முழு)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    இல்லையெனில்
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, origin, region, offset, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

#எனில் defined(HAVE_OPENCL_D3D11_NV)
நிலையான வெறுமை __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    முழு textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(Size(desc.Width, desc.Height), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &status);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#endif

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ifdef HAVE_DIRECTX_NV12
    எனில் (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        எனில் (!ocl::ocl_convert_nv12_to_bgr(clImage, clImageUV, clBuffer, (முழு)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImageUV, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    இல்லையெனில்
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, origin, region, offset, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    எனில்(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

வெறுமை convertToD3D11Texture2D(உள்ளீடு_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_UNUSED(src); CV_UNUSED(pD3D11Texture2D);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#இல்லையெனில்

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ifdef HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    எனில் (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        திரும்பு;
    }
#endif
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    எனில் (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    இல்லையெனில் {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#endif
}

வெறுமை convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_UNUSED(pD3D11Texture2D); CV_UNUSED(dst);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#இல்லையெனில்

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ifdef HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    எனில் (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        திரும்பு;
    }
#endif
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    எனில் (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    இல்லையெனில் {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#endif
}

வெறுமை convertToD3D10Texture2D(உள்ளீடு_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_UNUSED(src); CV_UNUSED(pD3D10Texture2D);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    முழு srcType = src.type();
    முழு textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (முழு)desc.Width && srcSize.height == (முழு)desc.Height);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &status);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D10ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    status = impl->clEnqueueReleaseD3D10ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#இல்லையெனில்
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

வெறுமை convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_UNUSED(pD3D10Texture2D); CV_UNUSED(dst);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    முழு textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(Size(desc.Width, desc.Height), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D10Texture2D, 0, &status);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D10ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    status = impl->clEnqueueReleaseD3D10ObjectsKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#இல்லையெனில்
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

வெறுமை convertToDirect3DSurface9(உள்ளீடு_Array src, IDirect3DSurface9* pDirect3DSurface9, வெறுமை* surfaceSharedHandle)
{
    CV_UNUSED(src); CV_UNUSED(pDirect3DSurface9); CV_UNUSED(surfaceSharedHandle);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    எனில் (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    முழு srcType = src.type();
    முழு surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (முழு)desc.Width && srcSize.height == (முழு)desc.Height);

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
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireDX9MediaSurfacesKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    status = impl->clEnqueueReleaseDX9MediaSurfacesKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#இல்லையெனில்
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

வெறுமை convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, வெறுமை* surfaceSharedHandle)
{
    CV_UNUSED(pDirect3DSurface9); CV_UNUSED(dst); CV_UNUSED(surfaceSharedHandle);
#எனில் !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#அல்லதெனில் defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    எனில் (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    எனில் (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    முழு surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(Size(desc.Width, desc.Height), surfaceType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_READ_ONLY,
            impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &status);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireDX9MediaSurfacesKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    status = impl->clEnqueueReleaseDX9MediaSurfacesKHR(q, 1, &clImage, 0, வெற்று, வெற்று);
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    status = clFinish(q); // TODO Use events
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    எனில் (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#இல்லையெனில்
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

} } // namespace cv::directx
