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

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ "opencv2/core.hpp"
#అన్తర్భావ "opencv2/core/ocl.hpp"
#అన్తర్భావ "opencv2/core/directx.hpp"
#అన్తర్భావ "opencl_kernels_core.hpp"

#నిర్వచితమైతే HAVE_DIRECTX
#అన్తర్భావ <vector>
#అన్తర్భావ "directx.inc.hpp"
#లేకపోతే // HAVE_DIRECTX
#నిర్వచించు NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#అయితే_ముగింపు

#నిర్వచితం_కాకపోతే HAVE_OPENCL
#నిర్వచించు NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#అయితే_ముగింపు // HAVE_OPENCL

ఉపయోగించి నామాకాశ cv::ocl;

నామాకాశ cv { నామాకాశ directx {

పూర్ణాంకం getTypeFromDXGI_FORMAT(స్థిరరాశి పూర్ణాంకం iDXGI_FORMAT)
{
    CV_వాడని(iDXGI_FORMAT);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం errorType = -1;
    ఎంపిక ((గణన DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    స్థితి DXGI_FORMAT_R32G32B32A32_FLOAT: తిరిగి CV_32FC4;
    స్థితి DXGI_FORMAT_R32G32B32A32_UINT:
    స్థితి DXGI_FORMAT_R32G32B32A32_SINT:  తిరిగి CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    స్థితి DXGI_FORMAT_R32G32B32_FLOAT: తిరిగి CV_32FC3;
    స్థితి DXGI_FORMAT_R32G32B32_UINT:
    స్థితి DXGI_FORMAT_R32G32B32_SINT: తిరిగి CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    స్థితి DXGI_FORMAT_R16G16B16A16_FLOAT: తిరిగి CV_16FC4;
    స్థితి DXGI_FORMAT_R16G16B16A16_UNORM:
    స్థితి DXGI_FORMAT_R16G16B16A16_UINT: తిరిగి CV_16UC4;
    స్థితి DXGI_FORMAT_R16G16B16A16_SNORM:
    స్థితి DXGI_FORMAT_R16G16B16A16_SINT: తిరిగి CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    స్థితి DXGI_FORMAT_R32G32_FLOAT: తిరిగి CV_32FC2;
    స్థితి DXGI_FORMAT_R32G32_UINT:
    స్థితి DXGI_FORMAT_R32G32_SINT: తిరిగి CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    స్థితి DXGI_FORMAT_R8G8B8A8_UNORM:
    స్థితి DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    స్థితి DXGI_FORMAT_R8G8B8A8_UINT: తిరిగి CV_8UC4;
    స్థితి DXGI_FORMAT_R8G8B8A8_SNORM:
    స్థితి DXGI_FORMAT_R8G8B8A8_SINT: తిరిగి CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    స్థితి DXGI_FORMAT_R16G16_FLOAT: తిరిగి CV_16FC2;
    స్థితి DXGI_FORMAT_R16G16_UNORM:
    స్థితి DXGI_FORMAT_R16G16_UINT: తిరిగి CV_16UC2;
    స్థితి DXGI_FORMAT_R16G16_SNORM:
    స్థితి DXGI_FORMAT_R16G16_SINT: తిరిగి CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    స్థితి DXGI_FORMAT_D32_FLOAT:
    స్థితి DXGI_FORMAT_R32_FLOAT: తిరిగి CV_32FC1;
    స్థితి DXGI_FORMAT_R32_UINT:
    స్థితి DXGI_FORMAT_R32_SINT: తిరిగి CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    స్థితి DXGI_FORMAT_R8G8_UNORM:
    స్థితి DXGI_FORMAT_R8G8_UINT: తిరిగి CV_8UC2;
    స్థితి DXGI_FORMAT_R8G8_SNORM:
    స్థితి DXGI_FORMAT_R8G8_SINT: తిరిగి CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    స్థితి DXGI_FORMAT_R16_FLOAT: తిరిగి CV_16FC1;
    స్థితి DXGI_FORMAT_D16_UNORM:
    స్థితి DXGI_FORMAT_R16_UNORM:
    స్థితి DXGI_FORMAT_R16_UINT: తిరిగి CV_16UC1;
    స్థితి DXGI_FORMAT_R16_SNORM:
    స్థితి DXGI_FORMAT_R16_SINT: తిరిగి CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    స్థితి DXGI_FORMAT_R8_UNORM:
    స్థితి DXGI_FORMAT_R8_UINT: తిరిగి CV_8UC1;
    స్థితి DXGI_FORMAT_R8_SNORM:
    స్థితి DXGI_FORMAT_R8_SINT: తిరిగి CV_8SC1;
    స్థితి DXGI_FORMAT_A8_UNORM: తిరిగి CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    స్థితి DXGI_FORMAT_R8G8_B8G8_UNORM:
    స్థితి DXGI_FORMAT_G8R8_G8B8_UNORM: తిరిగి CV_8UC4;
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
    స్థితి DXGI_FORMAT_B8G8R8A8_UNORM:
    స్థితి DXGI_FORMAT_B8G8R8X8_UNORM: తిరిగి CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    స్థితి DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: తిరిగి CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    స్థితి DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: తిరిగి CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#నిర్వచితమైతే HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    స్థితి DXGI_FORMAT_NV12: తిరిగి CV_8UC3;
#అయితే_ముగింపు
    అప్రమేయ: ఆపు;
    }
    తిరిగి errorType;
#అయితే_ముగింపు
}

పూర్ణాంకం getTypeFromD3DFORMAT(స్థిరరాశి పూర్ణాంకం iD3DFORMAT)
{
    CV_వాడని(iD3DFORMAT);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేకపోతే
    స్థిరరాశి పూర్ణాంకం errorType = -1;
    ఎంపిక ((గణన _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    స్థితి D3DFMT_R8G8B8: తిరిగి CV_8UC3;
    స్థితి D3DFMT_A8R8G8B8:
    స్థితి D3DFMT_X8R8G8B8: తిరిగి CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    స్థితి D3DFMT_A8: తిరిగి CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    స్థితి D3DFMT_A8B8G8R8:
    స్థితి D3DFMT_X8B8G8R8: తిరిగి CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    స్థితి D3DFMT_A8P8: తిరిగి CV_8UC2;
    స్థితి D3DFMT_P8: తిరిగి CV_8UC1;

    స్థితి D3DFMT_L8: తిరిగి CV_8UC1;
    స్థితి D3DFMT_A8L8: తిరిగి CV_8UC2;
    //case D3DFMT_A4L4:

    స్థితి D3DFMT_V8U8: తిరిగి CV_8UC2;
    //case D3DFMT_L6V5U5:
    స్థితి D3DFMT_X8L8V8U8:
    స్థితి D3DFMT_Q8W8V8U8: తిరిగి CV_8UC4;
    స్థితి D3DFMT_V16U16: తిరిగి CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    స్థితి D3DFMT_D16_LOCKABLE: తిరిగి CV_16UC1;
    స్థితి D3DFMT_D32: తిరిగి CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    స్థితి D3DFMT_D16: తిరిగి CV_16UC1;

    స్థితి D3DFMT_D32F_LOCKABLE: తిరిగి CV_32FC1;
    అప్రమేయ: ఆపు;
    }
    తిరిగి errorType;
#అయితే_ముగింపు
}

#ఒకవేళ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#నిర్వచితమైతే HAVE_OPENCL_D3D11_NV
తరగతి OpenCL_D3D11_NV : బహిరంగ ocl::Context::UserContext
{
బహిరంగ:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clCreateFromD3D11Texture2DNV");
        cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV = (cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV = (cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        ఒకవేళ (!clCreateFromD3D11Texture2DNV || !cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV || !cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11_NV");
        }
        device->కూడు_Ref();
    }
    ~OpenCL_D3D11_NV() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DNV_fn clCreateFromD3D11Texture2DNV;
    cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV_fn cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV;
    cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV_fn cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV;
};
#అయితే_ముగింపు

తరగతి OpenCL_D3D11 : బహిరంగ ocl::Context::UserContext
{
బహిరంగ:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clCreateFromD3D11Texture2DKHR");
        cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR = (cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR = (cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        ఒకవేళ (!clCreateFromD3D11Texture2DKHR || !cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR || !cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11");
        }
        device->కూడు_Ref();
    }
    ~OpenCL_D3D11() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DKHR_fn clCreateFromD3D11Texture2DKHR;
    cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR_fn cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR;
    cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR_fn cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR;
};

తరగతి OpenCL_D3D9 : బహిరంగ ocl::Context::UserContext
{
బహిరంగ:
    OpenCL_D3D9(cl_platform_id platform, IDirect3DDevice9* _device, IDirect3DDevice9Ex* _deviceEx)
        : device(_device)
        , deviceEx(_deviceEx)
    {
        clCreateFromDX9MediaSurfaceKHR = (clCreateFromDX9MediaSurfaceKHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clCreateFromDX9MediaSurfaceKHR");
        cl_వరుసలో_చేర్చు_Acquire_DX9_Media_Surfaces_KHR = (cl_వరుసలో_చేర్చు_Acquire_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueAcquireDX9MediaSurfacesKHR");
        cl_వరుసలో_చేర్చు_Release_DX9_Media_Surfaces_KHR = (cl_వరుసలో_చేర్చు_Release_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueReleaseDX9MediaSurfacesKHR");
        ఒకవేళ (!clCreateFromDX9MediaSurfaceKHR || !cl_వరుసలో_చేర్చు_Acquire_DX9_Media_Surfaces_KHR || !cl_వరుసలో_చేర్చు_Release_DX9_Media_Surfaces_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        ఒకవేళ (device)
            device->కూడు_Ref();
        ఒకవేళ (deviceEx)
            deviceEx->కూడు_Ref();
    }
    ~OpenCL_D3D9() {
        ఒకవేళ (device)
            device->Release();
        ఒకవేళ (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    cl_వరుసలో_చేర్చు_Acquire_DX9_Media_Surfaces_KHR_fn cl_వరుసలో_చేర్చు_Acquire_DX9_Media_Surfaces_KHR;
    cl_వరుసలో_చేర్చు_Release_DX9_Media_Surfaces_KHR_fn cl_వరుసలో_చేర్చు_Release_DX9_Media_Surfaces_KHR;
};

తరగతి OpenCL_D3D10 : బహిరంగ ocl::Context::UserContext
{
బహిరంగ:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clCreateFromD3D10Texture2DKHR");
        cl_వరుసలో_చేర్చు_Acquire_D3_D10_Objects_KHR = (cl_వరుసలో_చేర్చు_Acquire_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        cl_వరుసలో_చేర్చు_Release_D3_D10_Objects_KHR = (cl_వరుసలో_చేర్చు_Release_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        ఒకవేళ (!clCreateFromD3D10Texture2DKHR || !cl_వరుసలో_చేర్చు_Acquire_D3_D10_Objects_KHR || !cl_వరుసలో_చేర్చు_Release_D3_D10_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D10");
        }
        device->కూడు_Ref();
    }
    ~OpenCL_D3D10() {
        device->Release();
    }
    ID3D10Device* device;
    clCreateFromD3D10Texture2DKHR_fn clCreateFromD3D10Texture2DKHR;
    cl_వరుసలో_చేర్చు_Acquire_D3_D10_Objects_KHR_fn cl_వరుసలో_చేర్చు_Acquire_D3_D10_Objects_KHR;
    cl_వరుసలో_చేర్చు_Release_D3_D10_Objects_KHR_fn cl_వరుసలో_చేర్చు_Release_D3_D10_Objects_KHR;
};
#అయితే_ముగింపు

నామాకాశ ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_వాడని(pD3D11Device);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేకపోతే
    cl_uint అంకె_Platforms;
    cl_int స్థితి_2 = clGetPlatformIDs(0, ఖాళీ, &అంకె_Platforms);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ఒకవేళ (అంకె_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(అంకె_Platforms);
    స్థితి_2 = clGetPlatformIDs(అంకె_Platforms, &platforms[0], ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)అంకె_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        పూర్ణాంకం దొరికింది = -1;
        cl_device_id device = ఖాళీ;
        cl_uint అంకె_Devices = 0;
        cl_context context = ఖాళీ;

#నిర్వచితమైతే HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        ఒకవేళ (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            చేయి {
                device = ఖాళీ;
                అంకె_Devices = 0;
                స్థితి_2 = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &అంకె_Devices);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    ఆపు;
                ఒకవేళ (అంకె_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    లేకపోతే
                    {
                        దొరికింది = i;
                    }
                }
            } అయితే (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            ఒకవేళ (దొరికింది < 0) చేయి {
                device = ఖాళీ;
                అంకె_Devices = 0;
                స్థితి_2 = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &అంకె_Devices);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    ఆపు;
                ఒకవేళ (అంకె_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    లేకపోతే
                    {
                        దొరికింది = i;
                    }
                }
            } అయితే (0);
            ఒకవేళ (దొరికింది >= 0) {
                OpenCLExecutionContext clExecCtx;
                ప్రయత్నించు
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().సమితి_User_Context(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                పట్టుకో (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    విసురు;
                }
                clExecCtx.bind();
                తిరిగి const_cast<Context&>(clExecCtx.getContext());
            }
        }
#అయితే_ముగింపు
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        ఒకవేళ (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            చేయి {

                device = ఖాళీ;
                అంకె_Devices = 0;

                స్థితి_2 = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &అంకె_Devices);

                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    ఆపు;
                ఒకవేళ (అంకె_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            ఖాళీ, ఖాళీ
                    };
                    context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    లేకపోతే
                    {
                        దొరికింది = i;
                    }
                }
            } అయితే (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            ఒకవేళ (దొరికింది < 0) చేయి {
                device = ఖాళీ;
                అంకె_Devices = 0;
                స్థితి_2 = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &అంకె_Devices);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    ఆపు;
                ఒకవేళ (అంకె_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            ఖాళీ, ఖాళీ
                    };
                    context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    లేకపోతే
                    {
                        దొరికింది = i;
                    }
                }
            } అయితే (0);

            ఒకవేళ (దొరికింది >= 0) {
                OpenCLExecutionContext clExecCtx;
                ప్రయత్నించు
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().సమితి_User_Context(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                పట్టుకో (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    విసురు;
                }
                clExecCtx.bind();
                తిరిగి const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#అయితే_ముగింపు
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_వాడని(pD3D10Device);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేకపోతే
    cl_uint అంకె_Platforms;
    cl_int స్థితి_2 = clGetPlatformIDs(0, ఖాళీ, &అంకె_Platforms);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ఒకవేళ (అంకె_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(అంకె_Platforms);
    స్థితి_2 = clGetPlatformIDs(అంకె_Platforms, &platforms[0], ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)అంకె_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        పూర్ణాంకం దొరికింది = -1;
        cl_device_id device = ఖాళీ;
        cl_uint అంకె_Devices = 0;
        cl_context context = ఖాళీ;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        ఒకవేళ (!clGetDeviceIDsFromD3D10KHR)
            కొనసాగు;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        చేయి {
            device = ఖాళీ;
            అంకె_Devices = 0;
            స్థితి_2 = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &అంకె_Devices);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                ఆపు;
            ఒకవేళ (అంకె_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ఖాళీ, ఖాళీ
                };
                context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                లేకపోతే
                {
                    దొరికింది = i;
                }
            }
        } అయితే (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        ఒకవేళ (దొరికింది < 0) చేయి
        {
            device = ఖాళీ;
            అంకె_Devices = 0;
            స్థితి_2 = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &అంకె_Devices);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                ఆపు;
            ఒకవేళ (అంకె_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ఖాళీ, ఖాళీ
                };
                context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                లేకపోతే
                {
                    దొరికింది = i;
                }
            }
        } అయితే (0);

        ఒకవేళ (దొరికింది >= 0) {
            OpenCLExecutionContext clExecCtx;
            ప్రయత్నించు
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().సమితి_User_Context(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            పట్టుకో (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                విసురు;
            }
            clExecCtx.bind();
            తిరిగి const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#అయితే_ముగింపు
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_వాడని(pDirect3DDevice9Ex);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేకపోతే
    cl_uint అంకె_Platforms;
    cl_int స్థితి_2 = clGetPlatformIDs(0, ఖాళీ, &అంకె_Platforms);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ఒకవేళ (అంకె_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(అంకె_Platforms);
    స్థితి_2 = clGetPlatformIDs(అంకె_Platforms, &platforms[0], ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)అంకె_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        పూర్ణాంకం దొరికింది = -1;
        cl_device_id device = ఖాళీ;
        cl_uint అంకె_Devices = 0;
        cl_context context = ఖాళీ;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        ఒకవేళ (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            కొనసాగు;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        చేయి {
            device = ఖాళీ;
            అంకె_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            స్థితి_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &అంకె_Devices);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                ఆపు;
            ఒకవేళ (అంకె_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ఖాళీ, ఖాళీ
                };
                context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                లేకపోతే
                {
                    దొరికింది = i;
                }
            }
        } అయితే (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ఒకవేళ (దొరికింది < 0) చేయి
        {
            device = ఖాళీ;
            అంకె_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            స్థితి_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &అంకె_Devices);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                ఆపు;
            ఒకవేళ (అంకె_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ఖాళీ, ఖాళీ
                };
                context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                లేకపోతే
                {
                    దొరికింది = i;
                }
            }
        } అయితే (0);

        ఒకవేళ (దొరికింది >= 0) {
            OpenCLExecutionContext clExecCtx;
            ప్రయత్నించు
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().సమితి_User_Context(std::make_shared<OpenCL_D3D9>(platform, శూన్యసూచిక, pDirect3DDevice9Ex));
            }
            పట్టుకో (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                విసురు;
            }
            clExecCtx.bind();
            తిరిగి const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#అయితే_ముగింపు
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_వాడని(pDirect3DDevice9);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేకపోతే
    cl_uint అంకె_Platforms;
    cl_int స్థితి_2 = clGetPlatformIDs(0, ఖాళీ, &అంకె_Platforms);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ఒకవేళ (అంకె_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(అంకె_Platforms);
    స్థితి_2 = clGetPlatformIDs(అంకె_Platforms, &platforms[0], ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)అంకె_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        పూర్ణాంకం దొరికింది = -1;
        cl_device_id device = ఖాళీ;
        cl_uint అంకె_Devices = 0;
        cl_context context = ఖాళీ;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_చిరునామా_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        ఒకవేళ (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            కొనసాగు;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        చేయి {
            device = ఖాళీ;
            అంకె_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            స్థితి_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &అంకె_Devices);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                ఆపు;
            ఒకవేళ (అంకె_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ఖాళీ, ఖాళీ
                };
                context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                లేకపోతే
                {
                    దొరికింది = i;
                    ఆపు;
                }
            }
        } అయితే (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ఒకవేళ (దొరికింది < 0) చేయి
        {
            device = ఖాళీ;
            అంకె_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            స్థితి_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &అంకె_Devices);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                ఆపు;
            ఒకవేళ (అంకె_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ఖాళీ, ఖాళీ
                };
                context = clCreateContext(properties, 1, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
                ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                లేకపోతే
                {
                    దొరికింది = i;
                    ఆపు;
                }
            }
        } అయితే (0);

        ఒకవేళ (దొరికింది >= 0) {
            OpenCLExecutionContext clExecCtx;
            ప్రయత్నించు
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().సమితి_User_Context(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, శూన్యసూచిక));
            }
            పట్టుకో (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                విసురు;
            }
            clExecCtx.bind();
            తిరిగి const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#అయితే_ముగింపు
}

} // namespace cv::ocl

} // namespace directx


నామాకాశ ocl {

#ఒకవేళ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#నిర్వచితమైతే HAVE_DIRECTX_NV12

స్థిర
తార్కిక ocl_convert_nv12_గమ్యం_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem cl_బఫరు,
    పూర్ణాంకం అడుగు,
    పూర్ణాంకం నిలువుల_సంఖ్య,
    పూర్ణాంకం అడ్డువరుసలు)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(clImageY, clImageUV, cl_బఫరు, అడుగు, నిలువుల_సంఖ్య, అడ్డువరుసలు);

    size_t globalsize[] = { (size_t)నిలువుల_సంఖ్య/2, (size_t)అడ్డువరుసలు/2 };
    తిరిగి k.నిరంతరం(2, globalsize, 0, అబద్ధం);
}


స్థిర
తార్కిక ocl_convert_bgr_గమ్యం_nv12(
    cl_mem cl_బఫరు,
    పూర్ణాంకం అడుగు,
    పూర్ణాంకం నిలువుల_సంఖ్య,
    పూర్ణాంకం అడ్డువరుసలు,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(cl_బఫరు, అడుగు, నిలువుల_సంఖ్య, అడ్డువరుసలు, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)నిలువుల_సంఖ్య/2, (size_t)అడ్డువరుసలు/2 };
    తిరిగి k.నిరంతరం(2, globalsize, 0, అబద్ధం);
}

#అయితే_ముగింపు // HAVE_DIRECTX_NV12
#అయితే_ముగింపు // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


నామాకాశ directx {

#ఒకవేళ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
స్థిర శూన్య __convertToD3D11Texture2DKHR(ఆదానం_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    పూర్ణాంకం srcType = src.type();
    పూర్ణాంకం textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    కొలత src_కొలత = src.size();
    CV_Assert(src_కొలత.వెడల్పు == (పూర్ణాంకం)desc.వెడల్పు_2 && src_కొలత.ఎత్తు == (పూర్ణాంకం)desc.ఎత్తు_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int స్థితి_2 = 0;
    cl_mem clImage = 0;
#నిర్వచితమైతే HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#అయితే_ముగింపు
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#అయితే_ముగింపు

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        ఒకవేళ(!ocl::ocl_convert_bgr_గమ్యం_nv12(cl_బఫరు, (పూర్ణాంకం)u.అడుగు[0], u.నిలువుల_సంఖ్య, u.అడ్డువరుసలు, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    లేకపోతే
#అయితే_ముగింపు
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1 };

        స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_బఫరు_గమ్యం_Image(q, cl_బఫరు, clImage, offset, origin, region, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = clReleaseMemObject(clImageUV);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#అయితే_ముగింపు
}
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_OPENCL_D3D11_NV)
స్థిర శూన్య __convertToD3D11Texture2DNV(ఆదానం_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    పూర్ణాంకం srcType = src.type();
    పూర్ణాంకం textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    కొలత src_కొలత = src.size();
    CV_Assert(src_కొలత.వెడల్పు == (పూర్ణాంకం)desc.వెడల్పు_2 && src_కొలత.ఎత్తు == (పూర్ణాంకం)desc.ఎత్తు_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int స్థితి_2 = 0;
    cl_mem clImage = 0;
#నిర్వచితమైతే HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#అయితే_ముగింపు
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#అయితే_ముగింపు
    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        ఒకవేళ(!ocl::ocl_convert_bgr_గమ్యం_nv12(cl_బఫరు, (పూర్ణాంకం)u.అడుగు[0], u.నిలువుల_సంఖ్య, u.అడ్డువరుసలు, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    లేకపోతే
#అయితే_ముగింపు
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1 };

        స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_బఫరు_గమ్యం_Image(q, cl_బఫరు, clImage, offset, origin, region, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = clReleaseMemObject(clImageUV);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#అయితే_ముగింపు
}
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
స్థిర శూన్య __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    పూర్ణాంకం textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(కొలత(desc.వెడల్పు_2, desc.ఎత్తు_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int స్థితి_2 = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#అయితే_ముగింపు

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();

    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        ఒకవేళ(!ocl::ocl_convert_nv12_గమ్యం_bgr(clImage, clImageUV, cl_బఫరు, (పూర్ణాంకం)u.అడుగు[0], u.నిలువుల_సంఖ్య, u.అడ్డువరుసలు))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    లేకపోతే
#అయితే_ముగింపు
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1 };

        స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_Image_గమ్యం_బఫరు(q, clImage, cl_బఫరు, origin, region, offset, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = clReleaseMemObject(clImageUV);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#అయితే_ముగింపు
}
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_OPENCL_D3D11_NV)
స్థిర శూన్య __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    పూర్ణాంకం textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(కొలత(desc.వెడల్పు_2, desc.ఎత్తు_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int స్థితి_2 = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#అయితే_ముగింపు

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        ఒకవేళ (!ocl::ocl_convert_nv12_గమ్యం_bgr(clImage, clImageUV, cl_బఫరు, (పూర్ణాంకం)u.అడుగు[0], u.నిలువుల_సంఖ్య, u.అడ్డువరుసలు))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    లేకపోతే
#అయితే_ముగింపు
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1 };

        స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_Image_గమ్యం_బఫరు(q, clImage, cl_బఫరు, origin, region, offset, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#నిర్వచితమైతే HAVE_DIRECTX_NV12
    ఒకవేళ(DXGI_FORMAT_NV12 == desc.Format)
    {
        స్థితి_2 = clReleaseMemObject(clImageUV);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#అయితే_ముగింపు
}
#అయితే_ముగింపు

శూన్య convert_గమ్యం_D3_D11_Texture2_D(ఆదానం_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_వాడని(src); CV_వాడని(pD3D11Texture2D);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేకపోతే

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#నిర్వచితమైతే HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ఒకవేళ (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        తిరిగి;
    }
#అయితే_ముగింపు
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ఒకవేళ (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    లేకపోతే {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#అయితే_ముగింపు
}

శూన్య convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_వాడని(pD3D11Texture2D); CV_వాడని(dst);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#లేకపోతే

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#నిర్వచితమైతే HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ఒకవేళ (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        తిరిగి;
    }
#అయితే_ముగింపు
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ఒకవేళ (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    లేకపోతే {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#అయితే_ముగింపు
}

శూన్య convert_గమ్యం_D3_D10_Texture2_D(ఆదానం_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_వాడని(src); CV_వాడని(pD3D10Texture2D);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    పూర్ణాంకం srcType = src.type();
    పూర్ణాంకం textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    కొలత src_కొలత = src.size();
    CV_Assert(src_కొలత.వెడల్పు == (పూర్ణాంకం)desc.వెడల్పు_2 && src_కొలత.ఎత్తు == (పూర్ణాంకం)desc.ఎత్తు_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int స్థితి_2 = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1};
    స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_బఫరు_గమ్యం_Image(q, cl_బఫరు, clImage, offset, dst_origin, region, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#లేకపోతే
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#అయితే_ముగింపు
}

శూన్య convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_వాడని(pD3D10Texture2D); CV_వాడని(dst);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    పూర్ణాంకం textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(కొలత(desc.వెడల్పు_2, desc.ఎత్తు_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int స్థితి_2 = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D10Texture2D, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1};
    స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_Image_గమ్యం_బఫరు(q, clImage, cl_బఫరు, src_origin, region, offset, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#లేకపోతే
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#అయితే_ముగింపు
}

శూన్య convert_గమ్యం_Direct3_DSurface9(ఆదానం_Array src, IDirect3DSurface9* pDirect3DSurface9, శూన్య* surfaceSharedHandle)
{
    CV_వాడని(src); CV_వాడని(pDirect3DSurface9); CV_వాడని(surfaceSharedHandle);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    ఒకవేళ (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    పూర్ణాంకం srcType = src.type();
    పూర్ణాంకం surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    కొలత src_కొలత = src.size();
    CV_Assert(src_కొలత.వెడల్పు == (పూర్ణాంకం)desc.వెడల్పు_2 && src_కొలత.ఎత్తు == (పూర్ణాంకం)desc.ఎత్తు_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int స్థితి_2 = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_WRITE_ONLY,
        impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1};
    స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_బఫరు_గమ్యం_Image(q, cl_బఫరు, clImage, offset, dst_origin, region, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#లేకపోతే
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#అయితే_ముగింపు
}

శూన్య convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, శూన్య* surfaceSharedHandle)
{
    CV_వాడని(pDirect3DSurface9); CV_వాడని(dst); CV_వాడని(surfaceSharedHandle);
#ఒకవేళ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#లేదావేళ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    ఒకవేళ (శూన్యసూచిక == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    ఒకవేళ (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    పూర్ణాంకం surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(కొలత(desc.వెడల్పు_2, desc.ఎత్తు_2), surfaceType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int స్థితి_2 = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_READ_ONLY,
            impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &స్థితి_2);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_వరుస q = (cl_command_వరుస)వరుస::getDefault().ptr();
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.నిలువుల_సంఖ్య, (size_t)u.అడ్డువరుసలు, 1};
    స్థితి_2 = cl_వరుసలో_చేర్చు_Copy_Image_గమ్యం_బఫరు(q, clImage, cl_బఫరు, src_origin, region, offset, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    స్థితి_2 = impl->cl_వరుసలో_చేర్చు_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ఖాళీ, ఖాళీ);
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    స్థితి_2 = cl_ముగింపు(q); // TODO Use events
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    స్థితి_2 = clReleaseMemObject(clImage); // TODO RAII
    ఒకవేళ (స్థితి_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#లేకపోతే
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#అయితే_ముగింపు
}

} } // namespace cv::directx
