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

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/ocl.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/directx.hpp"
#ਅਨ੍ਤਰ੍ਭਾਵ "opencl_kernels_core.hpp"

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX
#ਅਨ੍ਤਰ੍ਭਾਵ <vector>
#ਅਨ੍ਤਰ੍ਭਾਵ "directx.inc.hpp"
#ਨਹੀਂਤਾਂ // HAVE_DIRECTX
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#ਜੇ_ਅੰਤ

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#ਜੇ_ਅੰਤ // HAVE_OPENCL

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::ocl;

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ directx {

ਪੂਰਨ getTypeFromDXGI_FORMAT(ਅਚਲ ਪੂਰਨ iDXGI_FORMAT)
{
    CV_ਅਣਵਰਤਿਆ(iDXGI_FORMAT);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ errorType = -1;
    ਚੋਣ ((ਗਿਣਤੀ DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R32G32B32A32_FLOAT: ਵਾਪਸ CV_32FC4;
    ਹਾਲਤ DXGI_FORMAT_R32G32B32A32_UINT:
    ਹਾਲਤ DXGI_FORMAT_R32G32B32A32_SINT:  ਵਾਪਸ CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R32G32B32_FLOAT: ਵਾਪਸ CV_32FC3;
    ਹਾਲਤ DXGI_FORMAT_R32G32B32_UINT:
    ਹਾਲਤ DXGI_FORMAT_R32G32B32_SINT: ਵਾਪਸ CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R16G16B16A16_FLOAT: ਵਾਪਸ CV_16FC4;
    ਹਾਲਤ DXGI_FORMAT_R16G16B16A16_UNORM:
    ਹਾਲਤ DXGI_FORMAT_R16G16B16A16_UINT: ਵਾਪਸ CV_16UC4;
    ਹਾਲਤ DXGI_FORMAT_R16G16B16A16_SNORM:
    ਹਾਲਤ DXGI_FORMAT_R16G16B16A16_SINT: ਵਾਪਸ CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R32G32_FLOAT: ਵਾਪਸ CV_32FC2;
    ਹਾਲਤ DXGI_FORMAT_R32G32_UINT:
    ਹਾਲਤ DXGI_FORMAT_R32G32_SINT: ਵਾਪਸ CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R8G8B8A8_UNORM:
    ਹਾਲਤ DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    ਹਾਲਤ DXGI_FORMAT_R8G8B8A8_UINT: ਵਾਪਸ CV_8UC4;
    ਹਾਲਤ DXGI_FORMAT_R8G8B8A8_SNORM:
    ਹਾਲਤ DXGI_FORMAT_R8G8B8A8_SINT: ਵਾਪਸ CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R16G16_FLOAT: ਵਾਪਸ CV_16FC2;
    ਹਾਲਤ DXGI_FORMAT_R16G16_UNORM:
    ਹਾਲਤ DXGI_FORMAT_R16G16_UINT: ਵਾਪਸ CV_16UC2;
    ਹਾਲਤ DXGI_FORMAT_R16G16_SNORM:
    ਹਾਲਤ DXGI_FORMAT_R16G16_SINT: ਵਾਪਸ CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_D32_FLOAT:
    ਹਾਲਤ DXGI_FORMAT_R32_FLOAT: ਵਾਪਸ CV_32FC1;
    ਹਾਲਤ DXGI_FORMAT_R32_UINT:
    ਹਾਲਤ DXGI_FORMAT_R32_SINT: ਵਾਪਸ CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R8G8_UNORM:
    ਹਾਲਤ DXGI_FORMAT_R8G8_UINT: ਵਾਪਸ CV_8UC2;
    ਹਾਲਤ DXGI_FORMAT_R8G8_SNORM:
    ਹਾਲਤ DXGI_FORMAT_R8G8_SINT: ਵਾਪਸ CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R16_FLOAT: ਵਾਪਸ CV_16FC1;
    ਹਾਲਤ DXGI_FORMAT_D16_UNORM:
    ਹਾਲਤ DXGI_FORMAT_R16_UNORM:
    ਹਾਲਤ DXGI_FORMAT_R16_UINT: ਵਾਪਸ CV_16UC1;
    ਹਾਲਤ DXGI_FORMAT_R16_SNORM:
    ਹਾਲਤ DXGI_FORMAT_R16_SINT: ਵਾਪਸ CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_R8_UNORM:
    ਹਾਲਤ DXGI_FORMAT_R8_UINT: ਵਾਪਸ CV_8UC1;
    ਹਾਲਤ DXGI_FORMAT_R8_SNORM:
    ਹਾਲਤ DXGI_FORMAT_R8_SINT: ਵਾਪਸ CV_8SC1;
    ਹਾਲਤ DXGI_FORMAT_A8_UNORM: ਵਾਪਸ CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    ਹਾਲਤ DXGI_FORMAT_R8G8_B8G8_UNORM:
    ਹਾਲਤ DXGI_FORMAT_G8R8_G8B8_UNORM: ਵਾਪਸ CV_8UC4;
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
    ਹਾਲਤ DXGI_FORMAT_B8G8R8A8_UNORM:
    ਹਾਲਤ DXGI_FORMAT_B8G8R8X8_UNORM: ਵਾਪਸ CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: ਵਾਪਸ CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    ਹਾਲਤ DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: ਵਾਪਸ CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    ਹਾਲਤ DXGI_FORMAT_NV12: ਵਾਪਸ CV_8UC3;
#ਜੇ_ਅੰਤ
    ਮੂਲ: ਰੋਕੋ;
    }
    ਵਾਪਸ errorType;
#ਜੇ_ਅੰਤ
}

ਪੂਰਨ getTypeFromD3DFORMAT(ਅਚਲ ਪੂਰਨ iD3DFORMAT)
{
    CV_ਅਣਵਰਤਿਆ(iD3DFORMAT);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ
    ਅਚਲ ਪੂਰਨ errorType = -1;
    ਚੋਣ ((ਗਿਣਤੀ _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    ਹਾਲਤ D3DFMT_R8G8B8: ਵਾਪਸ CV_8UC3;
    ਹਾਲਤ D3DFMT_A8R8G8B8:
    ਹਾਲਤ D3DFMT_X8R8G8B8: ਵਾਪਸ CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    ਹਾਲਤ D3DFMT_A8: ਵਾਪਸ CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    ਹਾਲਤ D3DFMT_A8B8G8R8:
    ਹਾਲਤ D3DFMT_X8B8G8R8: ਵਾਪਸ CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    ਹਾਲਤ D3DFMT_A8P8: ਵਾਪਸ CV_8UC2;
    ਹਾਲਤ D3DFMT_P8: ਵਾਪਸ CV_8UC1;

    ਹਾਲਤ D3DFMT_L8: ਵਾਪਸ CV_8UC1;
    ਹਾਲਤ D3DFMT_A8L8: ਵਾਪਸ CV_8UC2;
    //case D3DFMT_A4L4:

    ਹਾਲਤ D3DFMT_V8U8: ਵਾਪਸ CV_8UC2;
    //case D3DFMT_L6V5U5:
    ਹਾਲਤ D3DFMT_X8L8V8U8:
    ਹਾਲਤ D3DFMT_Q8W8V8U8: ਵਾਪਸ CV_8UC4;
    ਹਾਲਤ D3DFMT_V16U16: ਵਾਪਸ CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    ਹਾਲਤ D3DFMT_D16_LOCKABLE: ਵਾਪਸ CV_16UC1;
    ਹਾਲਤ D3DFMT_D32: ਵਾਪਸ CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    ਹਾਲਤ D3DFMT_D16: ਵਾਪਸ CV_16UC1;

    ਹਾਲਤ D3DFMT_D32F_LOCKABLE: ਵਾਪਸ CV_32FC1;
    ਮੂਲ: ਰੋਕੋ;
    }
    ਵਾਪਸ errorType;
#ਜੇ_ਅੰਤ
}

#ਜੇ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_D3D11_NV
ਵਰਗ OpenCL_D3D11_NV : ਜਨਤਕ ocl::Context::UserContext
{
ਜਨਤਕ:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clCreateFromD3D11Texture2DNV");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        ਜੇ (!clCreateFromD3D11Texture2DNV || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11_NV");
        }
        device->ਜੋੜੋ_Ref();
    }
    ~OpenCL_D3D11_NV() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DNV_fn clCreateFromD3D11Texture2DNV;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV;
};
#ਜੇ_ਅੰਤ

ਵਰਗ OpenCL_D3D11 : ਜਨਤਕ ocl::Context::UserContext
{
ਜਨਤਕ:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clCreateFromD3D11Texture2DKHR");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        ਜੇ (!clCreateFromD3D11Texture2DKHR || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11");
        }
        device->ਜੋੜੋ_Ref();
    }
    ~OpenCL_D3D11() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DKHR_fn clCreateFromD3D11Texture2DKHR;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR;
};

ਵਰਗ OpenCL_D3D9 : ਜਨਤਕ ocl::Context::UserContext
{
ਜਨਤਕ:
    OpenCL_D3D9(cl_platform_id platform, IDirect3DDevice9* _device, IDirect3DDevice9Ex* _deviceEx)
        : device(_device)
        , deviceEx(_deviceEx)
    {
        clCreateFromDX9MediaSurfaceKHR = (clCreateFromDX9MediaSurfaceKHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clCreateFromDX9MediaSurfaceKHR");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_DX9_Media_Surfaces_KHR = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueAcquireDX9MediaSurfacesKHR");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_DX9_Media_Surfaces_KHR = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueReleaseDX9MediaSurfacesKHR");
        ਜੇ (!clCreateFromDX9MediaSurfaceKHR || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_DX9_Media_Surfaces_KHR || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_DX9_Media_Surfaces_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        ਜੇ (device)
            device->ਜੋੜੋ_Ref();
        ਜੇ (deviceEx)
            deviceEx->ਜੋੜੋ_Ref();
    }
    ~OpenCL_D3D9() {
        ਜੇ (device)
            device->Release();
        ਜੇ (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_DX9_Media_Surfaces_KHR_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_DX9_Media_Surfaces_KHR;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_DX9_Media_Surfaces_KHR_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_DX9_Media_Surfaces_KHR;
};

ਵਰਗ OpenCL_D3D10 : ਜਨਤਕ ocl::Context::UserContext
{
ਜਨਤਕ:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clCreateFromD3D10Texture2DKHR");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D10_Objects_KHR = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D10_Objects_KHR = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        ਜੇ (!clCreateFromD3D10Texture2DKHR || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D10_Objects_KHR || !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D10_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D10");
        }
        device->ਜੋੜੋ_Ref();
    }
    ~OpenCL_D3D10() {
        device->Release();
    }
    ID3D10Device* device;
    clCreateFromD3D10Texture2DKHR_fn clCreateFromD3D10Texture2DKHR;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D10_Objects_KHR_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D10_Objects_KHR;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D10_Objects_KHR_fn cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D10_Objects_KHR;
};
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_ਅਣਵਰਤਿਆ(pD3D11Device);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ
    cl_uint ਅੰਕ_Platforms;
    cl_int ਸਥਿਤੀ = clGetPlatformIDs(0, ਨਿਰਾ, &ਅੰਕ_Platforms);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ਜੇ (ਅੰਕ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ਅੰਕ_Platforms);
    ਸਥਿਤੀ = clGetPlatformIDs(ਅੰਕ_Platforms, &platforms[0], ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)ਅੰਕ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        ਪੂਰਨ ਮਿਲਿਆ = -1;
        cl_device_id device = ਨਿਰਾ;
        cl_uint ਅੰਕ_Devices = 0;
        cl_context context = ਨਿਰਾ;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        ਜੇ (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            ਕਰੋ {
                device = ਨਿਰਾ;
                ਅੰਕ_Devices = 0;
                ਸਥਿਤੀ = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &ਅੰਕ_Devices);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    ਰੋਕੋ;
                ਜੇ (ਅੰਕ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਮਿਲਿਆ = i;
                    }
                }
            } ਜਦੋਂ (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            ਜੇ (ਮਿਲਿਆ < 0) ਕਰੋ {
                device = ਨਿਰਾ;
                ਅੰਕ_Devices = 0;
                ਸਥਿਤੀ = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &ਅੰਕ_Devices);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    ਰੋਕੋ;
                ਜੇ (ਅੰਕ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਮਿਲਿਆ = i;
                    }
                }
            } ਜਦੋਂ (0);
            ਜੇ (ਮਿਲਿਆ >= 0) {
                OpenCLExecutionContext clExecCtx;
                ਕੋਸ਼ਿਸ਼
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().ਸੈੱਟ_User_Context(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                ਫੜੋ (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    ਸੁੱਟੋ;
                }
                clExecCtx.bind();
                ਵਾਪਸ const_cast<Context&>(clExecCtx.getContext());
            }
        }
#ਜੇ_ਅੰਤ
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        ਜੇ (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            ਕਰੋ {

                device = ਨਿਰਾ;
                ਅੰਕ_Devices = 0;

                ਸਥਿਤੀ = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &ਅੰਕ_Devices);

                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    ਰੋਕੋ;
                ਜੇ (ਅੰਕ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            ਨਿਰਾ, ਨਿਰਾ
                    };
                    context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਮਿਲਿਆ = i;
                    }
                }
            } ਜਦੋਂ (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            ਜੇ (ਮਿਲਿਆ < 0) ਕਰੋ {
                device = ਨਿਰਾ;
                ਅੰਕ_Devices = 0;
                ਸਥਿਤੀ = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &ਅੰਕ_Devices);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    ਰੋਕੋ;
                ਜੇ (ਅੰਕ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            ਨਿਰਾ, ਨਿਰਾ
                    };
                    context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ਨਹੀਂਤਾਂ
                    {
                        ਮਿਲਿਆ = i;
                    }
                }
            } ਜਦੋਂ (0);

            ਜੇ (ਮਿਲਿਆ >= 0) {
                OpenCLExecutionContext clExecCtx;
                ਕੋਸ਼ਿਸ਼
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().ਸੈੱਟ_User_Context(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                ਫੜੋ (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    ਸੁੱਟੋ;
                }
                clExecCtx.bind();
                ਵਾਪਸ const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ਜੇ_ਅੰਤ
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_ਅਣਵਰਤਿਆ(pD3D10Device);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ
    cl_uint ਅੰਕ_Platforms;
    cl_int ਸਥਿਤੀ = clGetPlatformIDs(0, ਨਿਰਾ, &ਅੰਕ_Platforms);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ਜੇ (ਅੰਕ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ਅੰਕ_Platforms);
    ਸਥਿਤੀ = clGetPlatformIDs(ਅੰਕ_Platforms, &platforms[0], ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)ਅੰਕ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        ਪੂਰਨ ਮਿਲਿਆ = -1;
        cl_device_id device = ਨਿਰਾ;
        cl_uint ਅੰਕ_Devices = 0;
        cl_context context = ਨਿਰਾ;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        ਜੇ (!clGetDeviceIDsFromD3D10KHR)
            ਜਾਰੀ;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        ਕਰੋ {
            device = ਨਿਰਾ;
            ਅੰਕ_Devices = 0;
            ਸਥਿਤੀ = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &ਅੰਕ_Devices);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                ਰੋਕੋ;
            ਜੇ (ਅੰਕ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ਨਿਰਾ, ਨਿਰਾ
                };
                context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਮਿਲਿਆ = i;
                }
            }
        } ਜਦੋਂ (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        ਜੇ (ਮਿਲਿਆ < 0) ਕਰੋ
        {
            device = ਨਿਰਾ;
            ਅੰਕ_Devices = 0;
            ਸਥਿਤੀ = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &ਅੰਕ_Devices);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                ਰੋਕੋ;
            ਜੇ (ਅੰਕ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ਨਿਰਾ, ਨਿਰਾ
                };
                context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਮਿਲਿਆ = i;
                }
            }
        } ਜਦੋਂ (0);

        ਜੇ (ਮਿਲਿਆ >= 0) {
            OpenCLExecutionContext clExecCtx;
            ਕੋਸ਼ਿਸ਼
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ਸੈੱਟ_User_Context(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            ਫੜੋ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ਸੁੱਟੋ;
            }
            clExecCtx.bind();
            ਵਾਪਸ const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ਜੇ_ਅੰਤ
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_ਅਣਵਰਤਿਆ(pDirect3DDevice9Ex);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ
    cl_uint ਅੰਕ_Platforms;
    cl_int ਸਥਿਤੀ = clGetPlatformIDs(0, ਨਿਰਾ, &ਅੰਕ_Platforms);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ਜੇ (ਅੰਕ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ਅੰਕ_Platforms);
    ਸਥਿਤੀ = clGetPlatformIDs(ਅੰਕ_Platforms, &platforms[0], ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)ਅੰਕ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        ਪੂਰਨ ਮਿਲਿਆ = -1;
        cl_device_id device = ਨਿਰਾ;
        cl_uint ਅੰਕ_Devices = 0;
        cl_context context = ਨਿਰਾ;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        ਜੇ (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            ਜਾਰੀ;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ਕਰੋ {
            device = ਨਿਰਾ;
            ਅੰਕ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            ਸਥਿਤੀ = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ਅੰਕ_Devices);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                ਰੋਕੋ;
            ਜੇ (ਅੰਕ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ਨਿਰਾ, ਨਿਰਾ
                };
                context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਮਿਲਿਆ = i;
                }
            }
        } ਜਦੋਂ (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ਜੇ (ਮਿਲਿਆ < 0) ਕਰੋ
        {
            device = ਨਿਰਾ;
            ਅੰਕ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            ਸਥਿਤੀ = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ਅੰਕ_Devices);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                ਰੋਕੋ;
            ਜੇ (ਅੰਕ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ਨਿਰਾ, ਨਿਰਾ
                };
                context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਮਿਲਿਆ = i;
                }
            }
        } ਜਦੋਂ (0);

        ਜੇ (ਮਿਲਿਆ >= 0) {
            OpenCLExecutionContext clExecCtx;
            ਕੋਸ਼ਿਸ਼
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ਸੈੱਟ_User_Context(std::make_shared<OpenCL_D3D9>(platform, ਖਾਲੀ_ਸੂਚਕ, pDirect3DDevice9Ex));
            }
            ਫੜੋ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ਸੁੱਟੋ;
            }
            clExecCtx.bind();
            ਵਾਪਸ const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ਜੇ_ਅੰਤ
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_ਅਣਵਰਤਿਆ(pDirect3DDevice9);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ
    cl_uint ਅੰਕ_Platforms;
    cl_int ਸਥਿਤੀ = clGetPlatformIDs(0, ਨਿਰਾ, &ਅੰਕ_Platforms);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ਜੇ (ਅੰਕ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ਅੰਕ_Platforms);
    ਸਥਿਤੀ = clGetPlatformIDs(ਅੰਕ_Platforms, &platforms[0], ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)ਅੰਕ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        ਪੂਰਨ ਮਿਲਿਆ = -1;
        cl_device_id device = ਨਿਰਾ;
        cl_uint ਅੰਕ_Devices = 0;
        cl_context context = ਨਿਰਾ;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_ਪਤਾ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        ਜੇ (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            ਜਾਰੀ;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ਕਰੋ {
            device = ਨਿਰਾ;
            ਅੰਕ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            ਸਥਿਤੀ = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ਅੰਕ_Devices);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                ਰੋਕੋ;
            ਜੇ (ਅੰਕ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ਨਿਰਾ, ਨਿਰਾ
                };
                context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਮਿਲਿਆ = i;
                    ਰੋਕੋ;
                }
            }
        } ਜਦੋਂ (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ਜੇ (ਮਿਲਿਆ < 0) ਕਰੋ
        {
            device = ਨਿਰਾ;
            ਅੰਕ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            ਸਥਿਤੀ = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ਅੰਕ_Devices);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                ਰੋਕੋ;
            ਜੇ (ਅੰਕ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ਨਿਰਾ, ਨਿਰਾ
                };
                context = clCreateContext(properties, 1, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
                ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ਨਹੀਂਤਾਂ
                {
                    ਮਿਲਿਆ = i;
                    ਰੋਕੋ;
                }
            }
        } ਜਦੋਂ (0);

        ਜੇ (ਮਿਲਿਆ >= 0) {
            OpenCLExecutionContext clExecCtx;
            ਕੋਸ਼ਿਸ਼
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ਸੈੱਟ_User_Context(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, ਖਾਲੀ_ਸੂਚਕ));
            }
            ਫੜੋ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ਸੁੱਟੋ;
            }
            clExecCtx.bind();
            ਵਾਪਸ const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ਜੇ_ਅੰਤ
}

} // namespace cv::ocl

} // namespace directx


ਨਾਮਾਕਾਸ਼ ocl {

#ਜੇ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12

ਸਥਿਰ
ਤਾਰਕਿਕ ocl_convert_nv12_ਮੰਜ਼ਿਲ_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem cl_ਬਫਰ,
    ਪੂਰਨ ਕਦਮ,
    ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ,
    ਪੂਰਨ ਪੰਕਤੀਆਂ)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(clImageY, clImageUV, cl_ਬਫਰ, ਕਦਮ, ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ);

    size_t globalsize[] = { (size_t)ਖਾਨੇ_ਗਿਣਤੀ/2, (size_t)ਪੰਕਤੀਆਂ/2 };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, 0, ਝੂਠ);
}


ਸਥਿਰ
ਤਾਰਕਿਕ ocl_convert_bgr_ਮੰਜ਼ਿਲ_nv12(
    cl_mem cl_ਬਫਰ,
    ਪੂਰਨ ਕਦਮ,
    ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ,
    ਪੂਰਨ ਪੰਕਤੀਆਂ,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(cl_ਬਫਰ, ਕਦਮ, ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)ਖਾਨੇ_ਗਿਣਤੀ/2, (size_t)ਪੰਕਤੀਆਂ/2 };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, 0, ਝੂਠ);
}

#ਜੇ_ਅੰਤ // HAVE_DIRECTX_NV12
#ਜੇ_ਅੰਤ // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


ਨਾਮਾਕਾਸ਼ directx {

#ਜੇ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ਸਥਿਰ ਖਾਲੀ __convertToD3D11Texture2DKHR(ਨਿਵੇਸ਼_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ਪੂਰਨ srcType = src.type();
    ਪੂਰਨ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ਮਾਪ src_ਮਾਪ = src.size();
    CV_Assert(src_ਮਾਪ.ਚੌੜਾਈ == (ਪੂਰਨ)desc.ਚੌੜਾਈ_2 && src_ਮਾਪ.ਉਚਾਈ == (ਪੂਰਨ)desc.ਉਚਾਈ_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#ਜੇ_ਅੰਤ
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#ਜੇ_ਅੰਤ

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        ਜੇ(!ocl::ocl_convert_bgr_ਮੰਜ਼ਿਲ_nv12(cl_ਬਫਰ, (ਪੂਰਨ)u.ਕਦਮ[0], u.ਖਾਨੇ_ਗਿਣਤੀ, u.ਪੰਕਤੀਆਂ, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1 };

        ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_ਮੰਜ਼ਿਲ_Image(q, cl_ਬਫਰ, clImage, offset, origin, region, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = clReleaseMemObject(clImageUV);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_OPENCL_D3D11_NV)
ਸਥਿਰ ਖਾਲੀ __convertToD3D11Texture2DNV(ਨਿਵੇਸ਼_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ਪੂਰਨ srcType = src.type();
    ਪੂਰਨ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ਮਾਪ src_ਮਾਪ = src.size();
    CV_Assert(src_ਮਾਪ.ਚੌੜਾਈ == (ਪੂਰਨ)desc.ਚੌੜਾਈ_2 && src_ਮਾਪ.ਉਚਾਈ == (ਪੂਰਨ)desc.ਉਚਾਈ_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = 0;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#ਜੇ_ਅੰਤ
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#ਜੇ_ਅੰਤ
    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        ਜੇ(!ocl::ocl_convert_bgr_ਮੰਜ਼ਿਲ_nv12(cl_ਬਫਰ, (ਪੂਰਨ)u.ਕਦਮ[0], u.ਖਾਨੇ_ਗਿਣਤੀ, u.ਪੰਕਤੀਆਂ, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1 };

        ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_ਮੰਜ਼ਿਲ_Image(q, cl_ਬਫਰ, clImage, offset, origin, region, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = clReleaseMemObject(clImageUV);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ਸਥਿਰ ਖਾਲੀ __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ਪੂਰਨ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ਮਾਪ(desc.ਚੌੜਾਈ_2, desc.ਉਚਾਈ_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#ਜੇ_ਅੰਤ

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();

    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        ਜੇ(!ocl::ocl_convert_nv12_ਮੰਜ਼ਿਲ_bgr(clImage, clImageUV, cl_ਬਫਰ, (ਪੂਰਨ)u.ਕਦਮ[0], u.ਖਾਨੇ_ਗਿਣਤੀ, u.ਪੰਕਤੀਆਂ))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1 };

        ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_Image_ਮੰਜ਼ਿਲ_ਬਫਰ(q, clImage, cl_ਬਫਰ, origin, region, offset, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = clReleaseMemObject(clImageUV);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_OPENCL_D3D11_NV)
ਸਥਿਰ ਖਾਲੀ __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ਪੂਰਨ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ਮਾਪ(desc.ਚੌੜਾਈ_2, desc.ਉਚਾਈ_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#ਜੇ_ਅੰਤ

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        ਜੇ (!ocl::ocl_convert_nv12_ਮੰਜ਼ਿਲ_bgr(clImage, clImageUV, cl_ਬਫਰ, (ਪੂਰਨ)u.ਕਦਮ[0], u.ਖਾਨੇ_ਗਿਣਤੀ, u.ਪੰਕਤੀਆਂ))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    ਨਹੀਂਤਾਂ
#ਜੇ_ਅੰਤ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1 };

        ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_Image_ਮੰਜ਼ਿਲ_ਬਫਰ(q, clImage, cl_ਬਫਰ, origin, region, offset, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_DIRECTX_NV12
    ਜੇ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ਸਥਿਤੀ = clReleaseMemObject(clImageUV);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ਜੇ_ਅੰਤ
}
#ਜੇ_ਅੰਤ

ਖਾਲੀ convert_ਮੰਜ਼ਿਲ_D3_D11_Texture2_D(ਨਿਵੇਸ਼_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(pD3D11Texture2D);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ਜੇ (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ਜੇ (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    ਨਹੀਂਤਾਂ {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_ਅਣਵਰਤਿਆ(pD3D11Texture2D); CV_ਅਣਵਰਤਿਆ(dst);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ਜੇ (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        ਵਾਪਸ;
    }
#ਜੇ_ਅੰਤ
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ਜੇ (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    ਨਹੀਂਤਾਂ {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ convert_ਮੰਜ਼ਿਲ_D3_D10_Texture2_D(ਨਿਵੇਸ਼_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(pD3D10Texture2D);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    ਪੂਰਨ srcType = src.type();
    ਪੂਰਨ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ਮਾਪ src_ਮਾਪ = src.size();
    CV_Assert(src_ਮਾਪ.ਚੌੜਾਈ == (ਪੂਰਨ)desc.ਚੌੜਾਈ_2 && src_ਮਾਪ.ਉਚਾਈ == (ਪੂਰਨ)desc.ਉਚਾਈ_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1};
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_ਮੰਜ਼ਿਲ_Image(q, cl_ਬਫਰ, clImage, offset, dst_origin, region, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ਨਹੀਂਤਾਂ
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_ਅਣਵਰਤਿਆ(pD3D10Texture2D); CV_ਅਣਵਰਤਿਆ(dst);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    ਪੂਰਨ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ਮਾਪ(desc.ਚੌੜਾਈ_2, desc.ਉਚਾਈ_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ਸਥਿਤੀ = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D10Texture2D, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1};
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_Image_ਮੰਜ਼ਿਲ_ਬਫਰ(q, clImage, cl_ਬਫਰ, src_origin, region, offset, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ਨਹੀਂਤਾਂ
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ convert_ਮੰਜ਼ਿਲ_Direct3_DSurface9(ਨਿਵੇਸ਼_Array src, IDirect3DSurface9* pDirect3DSurface9, ਖਾਲੀ* surfaceSharedHandle)
{
    CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(pDirect3DSurface9); CV_ਅਣਵਰਤਿਆ(surfaceSharedHandle);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    ਜੇ (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    ਪੂਰਨ srcType = src.type();
    ਪੂਰਨ surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    ਮਾਪ src_ਮਾਪ = src.size();
    CV_Assert(src_ਮਾਪ.ਚੌੜਾਈ == (ਪੂਰਨ)desc.ਚੌੜਾਈ_2 && src_ਮਾਪ.ਉਚਾਈ == (ਪੂਰਨ)desc.ਉਚਾਈ_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ਸਥਿਤੀ = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_WRITE_ONLY,
        impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1};
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_ਬਫਰ_ਮੰਜ਼ਿਲ_Image(q, cl_ਬਫਰ, clImage, offset, dst_origin, region, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ਨਹੀਂਤਾਂ
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#ਜੇ_ਅੰਤ
}

ਖਾਲੀ convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, ਖਾਲੀ* surfaceSharedHandle)
{
    CV_ਅਣਵਰਤਿਆ(pDirect3DSurface9); CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(surfaceSharedHandle);
#ਜੇ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ਜਾਂਜੇ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    ਜੇ (ਖਾਲੀ_ਸੂਚਕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    ਜੇ (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    ਪੂਰਨ surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ਮਾਪ(desc.ਚੌੜਾਈ_2, desc.ਉਚਾਈ_2), surfaceType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ਸਥਿਤੀ = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_READ_ONLY,
            impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &ਸਥਿਤੀ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ਕਤਾਰ::getDefault().ptr();
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ਖਾਨੇ_ਗਿਣਤੀ, (size_t)u.ਪੰਕਤੀਆਂ, 1};
    ਸਥਿਤੀ = cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Copy_Image_ਮੰਜ਼ਿਲ_ਬਫਰ(q, clImage, cl_ਬਫਰ, src_origin, region, offset, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    ਸਥਿਤੀ = impl->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ਨਿਰਾ, ਨਿਰਾ);
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ਸਥਿਤੀ = clReleaseMemObject(clImage); // TODO RAII
    ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ਨਹੀਂਤਾਂ
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#ਜੇ_ਅੰਤ
}

} } // namespace cv::directx
