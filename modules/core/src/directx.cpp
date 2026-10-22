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

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव "opencv2/core.hpp"
#अन्तर्भाव "opencv2/core/ocl.hpp"
#अन्तर्भाव "opencv2/core/directx.hpp"
#अन्तर्भाव "opencl_kernels_core.hpp"

#ifdef HAVE_DIRECTX
#अन्तर्भाव <vector>
#अन्तर्भाव "directx.inc.hpp"
#अञ्ञथा // HAVE_DIRECTX
#define NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#endif

#ifndef HAVE_OPENCL
#define NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#endif // HAVE_OPENCL

using नामाकाश cv::ocl;

नामाकाश cv { नामाकाश directx {

पूर्णाङ्क getTypeFromDXGI_FORMAT(ध्रुव पूर्णाङ्क iDXGI_FORMAT)
{
    CV_UNUSED(iDXGI_FORMAT);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#अञ्ञथा
    ध्रुव पूर्णाङ्क errorType = -1;
    विकल्प ((गणना DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    पक्ष DXGI_FORMAT_R32G32B32A32_FLOAT: पटिनिवत्त CV_32FC4;
    पक्ष DXGI_FORMAT_R32G32B32A32_UINT:
    पक्ष DXGI_FORMAT_R32G32B32A32_SINT:  पटिनिवत्त CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    पक्ष DXGI_FORMAT_R32G32B32_FLOAT: पटिनिवत्त CV_32FC3;
    पक्ष DXGI_FORMAT_R32G32B32_UINT:
    पक्ष DXGI_FORMAT_R32G32B32_SINT: पटिनिवत्त CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    पक्ष DXGI_FORMAT_R16G16B16A16_FLOAT: पटिनिवत्त CV_16FC4;
    पक्ष DXGI_FORMAT_R16G16B16A16_UNORM:
    पक्ष DXGI_FORMAT_R16G16B16A16_UINT: पटिनिवत्त CV_16UC4;
    पक्ष DXGI_FORMAT_R16G16B16A16_SNORM:
    पक्ष DXGI_FORMAT_R16G16B16A16_SINT: पटिनिवत्त CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    पक्ष DXGI_FORMAT_R32G32_FLOAT: पटिनिवत्त CV_32FC2;
    पक्ष DXGI_FORMAT_R32G32_UINT:
    पक्ष DXGI_FORMAT_R32G32_SINT: पटिनिवत्त CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    पक्ष DXGI_FORMAT_R8G8B8A8_UNORM:
    पक्ष DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    पक्ष DXGI_FORMAT_R8G8B8A8_UINT: पटिनिवत्त CV_8UC4;
    पक्ष DXGI_FORMAT_R8G8B8A8_SNORM:
    पक्ष DXGI_FORMAT_R8G8B8A8_SINT: पटिनिवत्त CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    पक्ष DXGI_FORMAT_R16G16_FLOAT: पटिनिवत्त CV_16FC2;
    पक्ष DXGI_FORMAT_R16G16_UNORM:
    पक्ष DXGI_FORMAT_R16G16_UINT: पटिनिवत्त CV_16UC2;
    पक्ष DXGI_FORMAT_R16G16_SNORM:
    पक्ष DXGI_FORMAT_R16G16_SINT: पटिनिवत्त CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    पक्ष DXGI_FORMAT_D32_FLOAT:
    पक्ष DXGI_FORMAT_R32_FLOAT: पटिनिवत्त CV_32FC1;
    पक्ष DXGI_FORMAT_R32_UINT:
    पक्ष DXGI_FORMAT_R32_SINT: पटिनिवत्त CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    पक्ष DXGI_FORMAT_R8G8_UNORM:
    पक्ष DXGI_FORMAT_R8G8_UINT: पटिनिवत्त CV_8UC2;
    पक्ष DXGI_FORMAT_R8G8_SNORM:
    पक्ष DXGI_FORMAT_R8G8_SINT: पटिनिवत्त CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    पक्ष DXGI_FORMAT_R16_FLOAT: पटिनिवत्त CV_16FC1;
    पक्ष DXGI_FORMAT_D16_UNORM:
    पक्ष DXGI_FORMAT_R16_UNORM:
    पक्ष DXGI_FORMAT_R16_UINT: पटिनिवत्त CV_16UC1;
    पक्ष DXGI_FORMAT_R16_SNORM:
    पक्ष DXGI_FORMAT_R16_SINT: पटिनिवत्त CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    पक्ष DXGI_FORMAT_R8_UNORM:
    पक्ष DXGI_FORMAT_R8_UINT: पटिनिवत्त CV_8UC1;
    पक्ष DXGI_FORMAT_R8_SNORM:
    पक्ष DXGI_FORMAT_R8_SINT: पटिनिवत्त CV_8SC1;
    पक्ष DXGI_FORMAT_A8_UNORM: पटिनिवत्त CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    पक्ष DXGI_FORMAT_R8G8_B8G8_UNORM:
    पक्ष DXGI_FORMAT_G8R8_G8B8_UNORM: पटिनिवत्त CV_8UC4;
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
    पक्ष DXGI_FORMAT_B8G8R8A8_UNORM:
    पक्ष DXGI_FORMAT_B8G8R8X8_UNORM: पटिनिवत्त CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    पक्ष DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: पटिनिवत्त CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    पक्ष DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: पटिनिवत्त CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#ifdef HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    पक्ष DXGI_FORMAT_NV12: पटिनिवत्त CV_8UC3;
#endif
    स्वभाव: भङ्ग;
    }
    पटिनिवत्त errorType;
#endif
}

पूर्णाङ्क getTypeFromD3DFORMAT(ध्रुव पूर्णाङ्क iD3DFORMAT)
{
    CV_UNUSED(iD3DFORMAT);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#अञ्ञथा
    ध्रुव पूर्णाङ्क errorType = -1;
    विकल्प ((गणना _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    पक्ष D3DFMT_R8G8B8: पटिनिवत्त CV_8UC3;
    पक्ष D3DFMT_A8R8G8B8:
    पक्ष D3DFMT_X8R8G8B8: पटिनिवत्त CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    पक्ष D3DFMT_A8: पटिनिवत्त CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    पक्ष D3DFMT_A8B8G8R8:
    पक्ष D3DFMT_X8B8G8R8: पटिनिवत्त CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    पक्ष D3DFMT_A8P8: पटिनिवत्त CV_8UC2;
    पक्ष D3DFMT_P8: पटिनिवत्त CV_8UC1;

    पक्ष D3DFMT_L8: पटिनिवत्त CV_8UC1;
    पक्ष D3DFMT_A8L8: पटिनिवत्त CV_8UC2;
    //case D3DFMT_A4L4:

    पक्ष D3DFMT_V8U8: पटिनिवत्त CV_8UC2;
    //case D3DFMT_L6V5U5:
    पक्ष D3DFMT_X8L8V8U8:
    पक्ष D3DFMT_Q8W8V8U8: पटिनिवत्त CV_8UC4;
    पक्ष D3DFMT_V16U16: पटिनिवत्त CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    पक्ष D3DFMT_D16_LOCKABLE: पटिनिवत्त CV_16UC1;
    पक्ष D3DFMT_D32: पटिनिवत्त CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    पक्ष D3DFMT_D16: पटिनिवत्त CV_16UC1;

    पक्ष D3DFMT_D32F_LOCKABLE: पटिनिवत्त CV_32FC1;
    स्वभाव: भङ्ग;
    }
    पटिनिवत्त errorType;
#endif
}

#सचे defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#ifdef HAVE_OPENCL_D3D11_NV
वग्ग OpenCL_D3D11_NV : सार्वजनिक ocl::Context::UserContext
{
सार्वजनिक:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D11Texture2DNV");
        clEnqueueAcquireD3D11ObjectsNV = (clEnqueueAcquireD3D11ObjectsNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        clEnqueueReleaseD3D11ObjectsNV = (clEnqueueReleaseD3D11ObjectsNV_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        सचे (!clCreateFromD3D11Texture2DNV || !clEnqueueAcquireD3D11ObjectsNV || !clEnqueueReleaseD3D11ObjectsNV)
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

वग्ग OpenCL_D3D11 : सार्वजनिक ocl::Context::UserContext
{
सार्वजनिक:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D11Texture2DKHR");
        clEnqueueAcquireD3D11ObjectsKHR = (clEnqueueAcquireD3D11ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        clEnqueueReleaseD3D11ObjectsKHR = (clEnqueueReleaseD3D11ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        सचे (!clCreateFromD3D11Texture2DKHR || !clEnqueueAcquireD3D11ObjectsKHR || !clEnqueueReleaseD3D11ObjectsKHR)
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

वग्ग OpenCL_D3D9 : सार्वजनिक ocl::Context::UserContext
{
सार्वजनिक:
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
        सचे (!clCreateFromDX9MediaSurfaceKHR || !clEnqueueAcquireDX9MediaSurfacesKHR || !clEnqueueReleaseDX9MediaSurfacesKHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        सचे (device)
            device->AddRef();
        सचे (deviceEx)
            deviceEx->AddRef();
    }
    ~OpenCL_D3D9() {
        सचे (device)
            device->Release();
        सचे (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    clEnqueueAcquireDX9MediaSurfacesKHR_fn clEnqueueAcquireDX9MediaSurfacesKHR;
    clEnqueueReleaseDX9MediaSurfacesKHR_fn clEnqueueReleaseDX9MediaSurfacesKHR;
};

वग्ग OpenCL_D3D10 : सार्वजनिक ocl::Context::UserContext
{
सार्वजनिक:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromD3D10Texture2DKHR");
        clEnqueueAcquireD3D10ObjectsKHR = (clEnqueueAcquireD3D10ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        clEnqueueReleaseD3D10ObjectsKHR = (clEnqueueReleaseD3D10ObjectsKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        सचे (!clCreateFromD3D10Texture2DKHR || !clEnqueueAcquireD3D10ObjectsKHR || !clEnqueueReleaseD3D10ObjectsKHR)
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

नामाकाश ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_UNUSED(pD3D11Device);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अञ्ञथा
    cl_uint अङ्कः_Platforms;
    cl_int status = clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    सचे (अङ्कः_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अङ्कः_Platforms);
    status = clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    अत्थाय (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)अङ्कः_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        पूर्णाङ्क found = -1;
        cl_device_id device = रिक्त;
        cl_uint अङ्कः_Devices = 0;
        cl_context context = रिक्त;

#ifdef HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        सचे (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            करोहि {
                device = रिक्त;
                अङ्कः_Devices = 0;
                status = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &अङ्कः_Devices);
                सचे (status != CL_SUCCESS)
                    भङ्ग;
                सचे (अङ्कः_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                    सचे (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अञ्ञथा
                    {
                        found = i;
                    }
                }
            } याव (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            सचे (found < 0) करोहि {
                device = रिक्त;
                अङ्कः_Devices = 0;
                status = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &अङ्कः_Devices);
                सचे (status != CL_SUCCESS)
                    भङ्ग;
                सचे (अङ्कः_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                    सचे (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अञ्ञथा
                    {
                        found = i;
                    }
                }
            } याव (0);
            सचे (found >= 0) {
                OpenCLExecutionContext clExecCtx;
                यत्न
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                गृहाण (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    क्षिप;
                }
                clExecCtx.bind();
                पटिनिवत्त const_cast<Context&>(clExecCtx.getContext());
            }
        }
#endif
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        सचे (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            करोहि {

                device = रिक्त;
                अङ्कः_Devices = 0;

                status = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &अङ्कः_Devices);

                सचे (status != CL_SUCCESS)
                    भङ्ग;
                सचे (अङ्कः_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            रिक्त, रिक्त
                    };
                    context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                    सचे (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अञ्ञथा
                    {
                        found = i;
                    }
                }
            } याव (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            सचे (found < 0) करोहि {
                device = रिक्त;
                अङ्कः_Devices = 0;
                status = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &अङ्कः_Devices);
                सचे (status != CL_SUCCESS)
                    भङ्ग;
                सचे (अङ्कः_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            रिक्त, रिक्त
                    };
                    context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                    सचे (status != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    अञ्ञथा
                    {
                        found = i;
                    }
                }
            } याव (0);

            सचे (found >= 0) {
                OpenCLExecutionContext clExecCtx;
                यत्न
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                गृहाण (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    क्षिप;
                }
                clExecCtx.bind();
                पटिनिवत्त const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_UNUSED(pD3D10Device);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अञ्ञथा
    cl_uint अङ्कः_Platforms;
    cl_int status = clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    सचे (अङ्कः_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अङ्कः_Platforms);
    status = clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    अत्थाय (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)अङ्कः_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        पूर्णाङ्क found = -1;
        cl_device_id device = रिक्त;
        cl_uint अङ्कः_Devices = 0;
        cl_context context = रिक्त;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        सचे (!clGetDeviceIDsFromD3D10KHR)
            अनुवर्त;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        करोहि {
            device = रिक्त;
            अङ्कः_Devices = 0;
            status = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &अङ्कः_Devices);
            सचे (status != CL_SUCCESS)
                भङ्ग;
            सचे (अङ्कः_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        रिक्त, रिक्त
                };
                context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                सचे (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अञ्ञथा
                {
                    found = i;
                }
            }
        } याव (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        सचे (found < 0) करोहि
        {
            device = रिक्त;
            अङ्कः_Devices = 0;
            status = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &अङ्कः_Devices);
            सचे (status != CL_SUCCESS)
                भङ्ग;
            सचे (अङ्कः_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        रिक्त, रिक्त
                };
                context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                सचे (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अञ्ञथा
                {
                    found = i;
                }
            }
        } याव (0);

        सचे (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            यत्न
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            गृहाण (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                क्षिप;
            }
            clExecCtx.bind();
            पटिनिवत्त const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_UNUSED(pDirect3DDevice9Ex);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अञ्ञथा
    cl_uint अङ्कः_Platforms;
    cl_int status = clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    सचे (अङ्कः_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अङ्कः_Platforms);
    status = clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    अत्थाय (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)अङ्कः_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        पूर्णाङ्क found = -1;
        cl_device_id device = रिक्त;
        cl_uint अङ्कः_Devices = 0;
        cl_context context = रिक्त;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        सचे (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            अनुवर्त;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        करोहि {
            device = रिक्त;
            अङ्कः_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अङ्कः_Devices);
            सचे (status != CL_SUCCESS)
                भङ्ग;
            सचे (अङ्कः_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        रिक्त, रिक्त
                };
                context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                सचे (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अञ्ञथा
                {
                    found = i;
                }
            }
        } याव (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        सचे (found < 0) करोहि
        {
            device = रिक्त;
            अङ्कः_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अङ्कः_Devices);
            सचे (status != CL_SUCCESS)
                भङ्ग;
            सचे (अङ्कः_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        रिक्त, रिक्त
                };
                context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                सचे (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अञ्ञथा
                {
                    found = i;
                }
            }
        } याव (0);

        सचे (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            यत्न
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D9>(platform, nullptr, pDirect3DDevice9Ex));
            }
            गृहाण (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                क्षिप;
            }
            clExecCtx.bind();
            पटिनिवत्त const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_UNUSED(pDirect3DDevice9);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अञ्ञथा
    cl_uint अङ्कः_Platforms;
    cl_int status = clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    सचे (अङ्कः_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(अङ्कः_Platforms);
    status = clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    अत्थाय (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)अङ्कः_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        पूर्णाङ्क found = -1;
        cl_device_id device = रिक्त;
        cl_uint अङ्कः_Devices = 0;
        cl_context context = रिक्त;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        सचे (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            अनुवर्त;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        करोहि {
            device = रिक्त;
            अङ्कः_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अङ्कः_Devices);
            सचे (status != CL_SUCCESS)
                भङ्ग;
            सचे (अङ्कः_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        रिक्त, रिक्त
                };
                context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                सचे (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अञ्ञथा
                {
                    found = i;
                    भङ्ग;
                }
            }
        } याव (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        सचे (found < 0) करोहि
        {
            device = रिक्त;
            अङ्कः_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            status = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &अङ्कः_Devices);
            सचे (status != CL_SUCCESS)
                भङ्ग;
            सचे (अङ्कः_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        रिक्त, रिक्त
                };
                context = clCreateContext(properties, 1, &device, रिक्त, रिक्त, &status);
                सचे (status != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                अञ्ञथा
                {
                    found = i;
                    भङ्ग;
                }
            }
        } याव (0);

        सचे (found >= 0) {
            OpenCLExecutionContext clExecCtx;
            यत्न
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, nullptr));
            }
            गृहाण (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                क्षिप;
            }
            clExecCtx.bind();
            पटिनिवत्त const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#endif
}

} // namespace cv::ocl

} // namespace directx


नामाकाश ocl {

#सचे defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#ifdef HAVE_DIRECTX_NV12

स्थिर
तार्किक ocl_convert_nv12_to_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem clBuffer,
    पूर्णाङ्क step,
    पूर्णाङ्क cols,
    पूर्णाङ्क rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    पटिनिवत्त k.run(2, globalsize, 0, मिच्छा);
}


स्थिर
तार्किक ocl_convert_bgr_to_nv12(
    cl_mem clBuffer,
    पूर्णाङ्क step,
    पूर्णाङ्क cols,
    पूर्णाङ्क rows,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    सचे (k.empty())
        पटिनिवत्त मिच्छा;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    पटिनिवत्त k.run(2, globalsize, 0, मिच्छा);
}

#endif // HAVE_DIRECTX_NV12
#endif // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


नामाकाश directx {

#सचे defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
स्थिर शून्य __convertToD3D11Texture2DKHR(निवेशः_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णाङ्क srcType = src.type();
    पूर्णाङ्क textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (पूर्णाङ्क)desc.Width && srcSize.height == (पूर्णाङ्क)desc.Height);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;
#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#endif
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &status);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &status);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#endif

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        सचे(!ocl::ocl_convert_bgr_to_nv12(clBuffer, (पूर्णाङ्क)u.step[0], u.cols, u.rows, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    अञ्ञथा
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, origin, region, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

#सचे defined(HAVE_OPENCL_D3D11_NV)
स्थिर शून्य __convertToD3D11Texture2DNV(निवेशः_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णाङ्क srcType = src.type();
    पूर्णाङ्क textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (पूर्णाङ्क)desc.Width && srcSize.height == (पूर्णाङ्क)desc.Height);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;
#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#endif
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &status);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ifdef HAVE_DIRECTX_NV12
    सचे (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &status);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#endif
    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        सचे(!ocl::ocl_convert_bgr_to_nv12(clBuffer, (पूर्णाङ्क)u.step[0], u.cols, u.rows, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    अञ्ञथा
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, origin, region, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

#सचे defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
स्थिर शून्य __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णाङ्क textureType = getTypeFromDXGI_FORMAT(desc.Format);
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
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &status);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &status);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#endif

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();

    status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsKHR(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        सचे(!ocl::ocl_convert_nv12_to_bgr(clImage, clImageUV, clBuffer, (पूर्णाङ्क)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    अञ्ञथा
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, origin, region, offset, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

#सचे defined(HAVE_OPENCL_D3D11_NV)
स्थिर शून्य __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    पूर्णाङ्क textureType = getTypeFromDXGI_FORMAT(desc.Format);
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
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int status = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &status);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ifdef HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &status);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#endif

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ifdef HAVE_DIRECTX_NV12
    सचे (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        status = impl->clEnqueueAcquireD3D11ObjectsNV(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        सचे (!ocl::ocl_convert_nv12_to_bgr(clImage, clImageUV, clBuffer, (पूर्णाङ्क)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImageUV, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    अञ्ञथा
#endif
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.cols, (size_t)u.rows, 1 };

        status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, origin, region, offset, 0, रिक्त, रिक्त);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    status = impl->clEnqueueReleaseD3D11ObjectsNV(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ifdef HAVE_DIRECTX_NV12
    सचे(DXGI_FORMAT_NV12 == desc.Format)
    {
        status = clReleaseMemObject(clImageUV);
        सचे (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#endif
}
#endif

शून्य convertToD3D11Texture2D(निवेशः_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_UNUSED(src); CV_UNUSED(pD3D11Texture2D);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अञ्ञथा

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ifdef HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    सचे (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        पटिनिवत्त;
    }
#endif
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    सचे (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    अञ्ञथा {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#endif
}

शून्य convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_UNUSED(pD3D11Texture2D); CV_UNUSED(dst);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#अञ्ञथा

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ifdef HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    सचे (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        पटिनिवत्त;
    }
#endif
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    सचे (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    अञ्ञथा {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#endif
}

शून्य convertToD3D10Texture2D(निवेशः_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_UNUSED(src); CV_UNUSED(pD3D10Texture2D);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    पूर्णाङ्क srcType = src.type();
    पूर्णाङ्क textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (पूर्णाङ्क)desc.Width && srcSize.height == (पूर्णाङ्क)desc.Height);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int status = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &status);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D10ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    status = impl->clEnqueueReleaseD3D10ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अञ्ञथा
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

शून्य convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_UNUSED(pD3D10Texture2D); CV_UNUSED(dst);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    पूर्णाङ्क textureType = getTypeFromDXGI_FORMAT(desc.Format);
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
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireD3D10ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    status = impl->clEnqueueReleaseD3D10ObjectsKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अञ्ञथा
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

शून्य convertToDirect3DSurface9(निवेशः_Array src, IDirect3DSurface9* pDirect3DSurface9, शून्य* surfaceSharedHandle)
{
    CV_UNUSED(src); CV_UNUSED(pDirect3DSurface9); CV_UNUSED(surfaceSharedHandle);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    सचे (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    पूर्णाङ्क srcType = src.type();
    पूर्णाङ्क surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == (पूर्णाङ्क)desc.Width && srcSize.height == (पूर्णाङ्क)desc.Height);

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
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireDX9MediaSurfacesKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyBufferToImage(q, clBuffer, clImage, offset, dst_origin, region, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    status = impl->clEnqueueReleaseDX9MediaSurfacesKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अञ्ञथा
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

शून्य convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, शून्य* surfaceSharedHandle)
{
    CV_UNUSED(pDirect3DSurface9); CV_UNUSED(dst); CV_UNUSED(surfaceSharedHandle);
#सचे !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#नोचेद्यदि defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    सचे (nullptr == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    सचे (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    पूर्णाङ्क surfaceType = getTypeFromD3DFORMAT(desc.Format);
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
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_queue q = (cl_command_queue)Queue::getDefault().ptr();
    status = impl->clEnqueueAcquireDX9MediaSurfacesKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.cols, (size_t)u.rows, 1};
    status = clEnqueueCopyImageToBuffer(q, clImage, clBuffer, src_origin, region, offset, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    status = impl->clEnqueueReleaseDX9MediaSurfacesKHR(q, 1, &clImage, 0, रिक्त, रिक्त);
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    status = clFinish(q); // TODO Use events
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    status = clReleaseMemObject(clImage); // TODO RAII
    सचे (status != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#अञ्ञथा
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#endif
}

} } // namespace cv::directx
