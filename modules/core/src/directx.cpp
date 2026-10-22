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

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ "opencv2/core.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/ocl.hpp"
#ಅನ್ತರ್ಭಾವ "opencv2/core/directx.hpp"
#ಅನ್ತರ್ಭಾವ "opencl_kernels_core.hpp"

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX
#ಅನ್ತರ್ಭಾವ <vector>
#ಅನ್ತರ್ಭಾವ "directx.inc.hpp"
#ಇಲ್ಲದಿದ್ದರೆ // HAVE_DIRECTX
#ವ್ಯಾಖ್ಯಾನಿಸು NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ HAVE_OPENCL
#ವ್ಯಾಖ್ಯಾನಿಸು NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENCL

ಬಳಸಿ ನಾಮಾಕಾಶ cv::ocl;

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ directx {

ಪೂರ್ಣಾಂಕ getTypeFromDXGI_FORMAT(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ iDXGI_FORMAT)
{
    CV_ಬಳಸದ(iDXGI_FORMAT);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ errorType = -1;
    ಆಯ್ಕೆ ((ಎಣಿಕೆ DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32B32A32_FLOAT: ಹಿಂತಿರುಗಿ CV_32FC4;
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32B32A32_UINT:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32B32A32_SINT:  ಹಿಂತಿರುಗಿ CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32B32_FLOAT: ಹಿಂತಿರುಗಿ CV_32FC3;
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32B32_UINT:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32B32_SINT: ಹಿಂತಿರುಗಿ CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16B16A16_FLOAT: ಹಿಂತಿರುಗಿ CV_16FC4;
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16B16A16_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16B16A16_UINT: ಹಿಂತಿರುಗಿ CV_16UC4;
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16B16A16_SNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16B16A16_SINT: ಹಿಂತಿರುಗಿ CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32_FLOAT: ಹಿಂತಿರುಗಿ CV_32FC2;
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32_UINT:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32G32_SINT: ಹಿಂತಿರುಗಿ CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8B8A8_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8B8A8_UINT: ಹಿಂತಿರುಗಿ CV_8UC4;
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8B8A8_SNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8B8A8_SINT: ಹಿಂತಿರುಗಿ CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16_FLOAT: ಹಿಂತಿರುಗಿ CV_16FC2;
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16_UINT: ಹಿಂತಿರುಗಿ CV_16UC2;
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16_SNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16G16_SINT: ಹಿಂತಿರುಗಿ CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_D32_FLOAT:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32_FLOAT: ಹಿಂತಿರುಗಿ CV_32FC1;
    ಸ್ಥಿತಿ DXGI_FORMAT_R32_UINT:
    ಸ್ಥಿತಿ DXGI_FORMAT_R32_SINT: ಹಿಂತಿರುಗಿ CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8_UINT: ಹಿಂತಿರುಗಿ CV_8UC2;
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8_SNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8_SINT: ಹಿಂತಿರುಗಿ CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16_FLOAT: ಹಿಂತಿರುಗಿ CV_16FC1;
    ಸ್ಥಿತಿ DXGI_FORMAT_D16_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16_UINT: ಹಿಂತಿರುಗಿ CV_16UC1;
    ಸ್ಥಿತಿ DXGI_FORMAT_R16_SNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R16_SINT: ಹಿಂತಿರುಗಿ CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8_UINT: ಹಿಂತಿರುಗಿ CV_8UC1;
    ಸ್ಥಿತಿ DXGI_FORMAT_R8_SNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8_SINT: ಹಿಂತಿರುಗಿ CV_8SC1;
    ಸ್ಥಿತಿ DXGI_FORMAT_A8_UNORM: ಹಿಂತಿರುಗಿ CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    ಸ್ಥಿತಿ DXGI_FORMAT_R8G8_B8G8_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_G8R8_G8B8_UNORM: ಹಿಂತಿರುಗಿ CV_8UC4;
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
    ಸ್ಥಿತಿ DXGI_FORMAT_B8G8R8A8_UNORM:
    ಸ್ಥಿತಿ DXGI_FORMAT_B8G8R8X8_UNORM: ಹಿಂತಿರುಗಿ CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: ಹಿಂತಿರುಗಿ CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    ಸ್ಥಿತಿ DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: ಹಿಂತಿರುಗಿ CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    ಸ್ಥಿತಿ DXGI_FORMAT_NV12: ಹಿಂತಿರುಗಿ CV_8UC3;
#ಆದರೆ_ಅಂತ್ಯ
    ಪೂರ್ವನಿಯೋಜಿತ: ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ errorType;
#ಆದರೆ_ಅಂತ್ಯ
}

ಪೂರ್ಣಾಂಕ getTypeFromD3DFORMAT(ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ iD3DFORMAT)
{
    CV_ಬಳಸದ(iD3DFORMAT);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ errorType = -1;
    ಆಯ್ಕೆ ((ಎಣಿಕೆ _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    ಸ್ಥಿತಿ D3DFMT_R8G8B8: ಹಿಂತಿರುಗಿ CV_8UC3;
    ಸ್ಥಿತಿ D3DFMT_A8R8G8B8:
    ಸ್ಥಿತಿ D3DFMT_X8R8G8B8: ಹಿಂತಿರುಗಿ CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    ಸ್ಥಿತಿ D3DFMT_A8: ಹಿಂತಿರುಗಿ CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    ಸ್ಥಿತಿ D3DFMT_A8B8G8R8:
    ಸ್ಥಿತಿ D3DFMT_X8B8G8R8: ಹಿಂತಿರುಗಿ CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    ಸ್ಥಿತಿ D3DFMT_A8P8: ಹಿಂತಿರುಗಿ CV_8UC2;
    ಸ್ಥಿತಿ D3DFMT_P8: ಹಿಂತಿರುಗಿ CV_8UC1;

    ಸ್ಥಿತಿ D3DFMT_L8: ಹಿಂತಿರುಗಿ CV_8UC1;
    ಸ್ಥಿತಿ D3DFMT_A8L8: ಹಿಂತಿರುಗಿ CV_8UC2;
    //case D3DFMT_A4L4:

    ಸ್ಥಿತಿ D3DFMT_V8U8: ಹಿಂತಿರುಗಿ CV_8UC2;
    //case D3DFMT_L6V5U5:
    ಸ್ಥಿತಿ D3DFMT_X8L8V8U8:
    ಸ್ಥಿತಿ D3DFMT_Q8W8V8U8: ಹಿಂತಿರುಗಿ CV_8UC4;
    ಸ್ಥಿತಿ D3DFMT_V16U16: ಹಿಂತಿರುಗಿ CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    ಸ್ಥಿತಿ D3DFMT_D16_LOCKABLE: ಹಿಂತಿರುಗಿ CV_16UC1;
    ಸ್ಥಿತಿ D3DFMT_D32: ಹಿಂತಿರುಗಿ CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    ಸ್ಥಿತಿ D3DFMT_D16: ಹಿಂತಿರುಗಿ CV_16UC1;

    ಸ್ಥಿತಿ D3DFMT_D32F_LOCKABLE: ಹಿಂತಿರುಗಿ CV_32FC1;
    ಪೂರ್ವನಿಯೋಜಿತ: ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ errorType;
#ಆದರೆ_ಅಂತ್ಯ
}

#ಆದರೆ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_D3D11_NV
ವರ್ಗ OpenCL_D3D11_NV : ಸಾರ್ವಜನಿಕ ocl::Context::UserContext
{
ಸಾರ್ವಜನಿಕ:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clCreateFromD3D11Texture2DNV");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        ಆದರೆ (!clCreateFromD3D11Texture2DNV || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11_NV");
        }
        device->ಕೂಡಿಸು_Ref();
    }
    ~OpenCL_D3D11_NV() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DNV_fn clCreateFromD3D11Texture2DNV;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV;
};
#ಆದರೆ_ಅಂತ್ಯ

ವರ್ಗ OpenCL_D3D11 : ಸಾರ್ವಜನಿಕ ocl::Context::UserContext
{
ಸಾರ್ವಜನಿಕ:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clCreateFromD3D11Texture2DKHR");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        ಆದರೆ (!clCreateFromD3D11Texture2DKHR || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11");
        }
        device->ಕೂಡಿಸು_Ref();
    }
    ~OpenCL_D3D11() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DKHR_fn clCreateFromD3D11Texture2DKHR;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR;
};

ವರ್ಗ OpenCL_D3D9 : ಸಾರ್ವಜನಿಕ ocl::Context::UserContext
{
ಸಾರ್ವಜನಿಕ:
    OpenCL_D3D9(cl_platform_id platform, IDirect3DDevice9* _device, IDirect3DDevice9Ex* _deviceEx)
        : device(_device)
        , deviceEx(_deviceEx)
    {
        clCreateFromDX9MediaSurfaceKHR = (clCreateFromDX9MediaSurfaceKHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clCreateFromDX9MediaSurfaceKHR");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_DX9_Media_Surfaces_KHR = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueAcquireDX9MediaSurfacesKHR");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_DX9_Media_Surfaces_KHR = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueReleaseDX9MediaSurfacesKHR");
        ಆದರೆ (!clCreateFromDX9MediaSurfaceKHR || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_DX9_Media_Surfaces_KHR || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_DX9_Media_Surfaces_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        ಆದರೆ (device)
            device->ಕೂಡಿಸು_Ref();
        ಆದರೆ (deviceEx)
            deviceEx->ಕೂಡಿಸು_Ref();
    }
    ~OpenCL_D3D9() {
        ಆದರೆ (device)
            device->Release();
        ಆದರೆ (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_DX9_Media_Surfaces_KHR_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_DX9_Media_Surfaces_KHR;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_DX9_Media_Surfaces_KHR_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_DX9_Media_Surfaces_KHR;
};

ವರ್ಗ OpenCL_D3D10 : ಸಾರ್ವಜನಿಕ ocl::Context::UserContext
{
ಸಾರ್ವಜನಿಕ:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clCreateFromD3D10Texture2DKHR");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D10_Objects_KHR = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D10_Objects_KHR = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        ಆದರೆ (!clCreateFromD3D10Texture2DKHR || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D10_Objects_KHR || !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D10_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D10");
        }
        device->ಕೂಡಿಸು_Ref();
    }
    ~OpenCL_D3D10() {
        device->Release();
    }
    ID3D10Device* device;
    clCreateFromD3D10Texture2DKHR_fn clCreateFromD3D10Texture2DKHR;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D10_Objects_KHR_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D10_Objects_KHR;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D10_Objects_KHR_fn cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D10_Objects_KHR;
};
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_ಬಳಸದ(pD3D11Device);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    cl_uint ಅಂಕೆ_Platforms;
    cl_int ಸ್ಥಿತಿ_2 = clGetPlatformIDs(0, ಖಾಲಿ, &ಅಂಕೆ_Platforms);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ಆದರೆ (ಅಂಕೆ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ಅಂಕೆ_Platforms);
    ಸ್ಥಿತಿ_2 = clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)ಅಂಕೆ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು = -1;
        cl_device_id device = ಖಾಲಿ;
        cl_uint ಅಂಕೆ_Devices = 0;
        cl_context context = ಖಾಲಿ;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        ಆದರೆ (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            ಮಾಡು {
                device = ಖಾಲಿ;
                ಅಂಕೆ_Devices = 0;
                ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &ಅಂಕೆ_Devices);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    ನಿಲ್ಲಿಸು;
                ಆದರೆ (ಅಂಕೆ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ಸಿಕ್ಕಿತು = i;
                    }
                }
            } ಆಗ (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            ಆದರೆ (ಸಿಕ್ಕಿತು < 0) ಮಾಡು {
                device = ಖಾಲಿ;
                ಅಂಕೆ_Devices = 0;
                ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &ಅಂಕೆ_Devices);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    ನಿಲ್ಲಿಸು;
                ಆದರೆ (ಅಂಕೆ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ಸಿಕ್ಕಿತು = i;
                    }
                }
            } ಆಗ (0);
            ಆದರೆ (ಸಿಕ್ಕಿತು >= 0) {
                OpenCLExecutionContext clExecCtx;
                ಪ್ರಯತ್ನಿಸು
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().ಗಣ_User_Context(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                ಹಿಡಿ (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    ಎಸೆ;
                }
                clExecCtx.bind();
                ಹಿಂತಿರುಗಿ const_cast<Context&>(clExecCtx.getContext());
            }
        }
#ಆದರೆ_ಅಂತ್ಯ
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        ಆದರೆ (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            ಮಾಡು {

                device = ಖಾಲಿ;
                ಅಂಕೆ_Devices = 0;

                ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &ಅಂಕೆ_Devices);

                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    ನಿಲ್ಲಿಸು;
                ಆದರೆ (ಅಂಕೆ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            ಖಾಲಿ, ಖಾಲಿ
                    };
                    context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ಸಿಕ್ಕಿತು = i;
                    }
                }
            } ಆಗ (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            ಆದರೆ (ಸಿಕ್ಕಿತು < 0) ಮಾಡು {
                device = ಖಾಲಿ;
                ಅಂಕೆ_Devices = 0;
                ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &ಅಂಕೆ_Devices);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    ನಿಲ್ಲಿಸು;
                ಆದರೆ (ಅಂಕೆ_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            ಖಾಲಿ, ಖಾಲಿ
                    };
                    context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ಇಲ್ಲದಿದ್ದರೆ
                    {
                        ಸಿಕ್ಕಿತು = i;
                    }
                }
            } ಆಗ (0);

            ಆದರೆ (ಸಿಕ್ಕಿತು >= 0) {
                OpenCLExecutionContext clExecCtx;
                ಪ್ರಯತ್ನಿಸು
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().ಗಣ_User_Context(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                ಹಿಡಿ (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    ಎಸೆ;
                }
                clExecCtx.bind();
                ಹಿಂತಿರುಗಿ const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ಆದರೆ_ಅಂತ್ಯ
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_ಬಳಸದ(pD3D10Device);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    cl_uint ಅಂಕೆ_Platforms;
    cl_int ಸ್ಥಿತಿ_2 = clGetPlatformIDs(0, ಖಾಲಿ, &ಅಂಕೆ_Platforms);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ಆದರೆ (ಅಂಕೆ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ಅಂಕೆ_Platforms);
    ಸ್ಥಿತಿ_2 = clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)ಅಂಕೆ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು = -1;
        cl_device_id device = ಖಾಲಿ;
        cl_uint ಅಂಕೆ_Devices = 0;
        cl_context context = ಖಾಲಿ;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        ಆದರೆ (!clGetDeviceIDsFromD3D10KHR)
            ಮುಂದುವರಿ;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        ಮಾಡು {
            device = ಖಾಲಿ;
            ಅಂಕೆ_Devices = 0;
            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &ಅಂಕೆ_Devices);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                ನಿಲ್ಲಿಸು;
            ಆದರೆ (ಅಂಕೆ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ಖಾಲಿ, ಖಾಲಿ
                };
                context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಸಿಕ್ಕಿತು = i;
                }
            }
        } ಆಗ (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        ಆದರೆ (ಸಿಕ್ಕಿತು < 0) ಮಾಡು
        {
            device = ಖಾಲಿ;
            ಅಂಕೆ_Devices = 0;
            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &ಅಂಕೆ_Devices);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                ನಿಲ್ಲಿಸು;
            ಆದರೆ (ಅಂಕೆ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ಖಾಲಿ, ಖಾಲಿ
                };
                context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಸಿಕ್ಕಿತು = i;
                }
            }
        } ಆಗ (0);

        ಆದರೆ (ಸಿಕ್ಕಿತು >= 0) {
            OpenCLExecutionContext clExecCtx;
            ಪ್ರಯತ್ನಿಸು
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ಗಣ_User_Context(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            ಹಿಡಿ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ಎಸೆ;
            }
            clExecCtx.bind();
            ಹಿಂತಿರುಗಿ const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ಆದರೆ_ಅಂತ್ಯ
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_ಬಳಸದ(pDirect3DDevice9Ex);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    cl_uint ಅಂಕೆ_Platforms;
    cl_int ಸ್ಥಿತಿ_2 = clGetPlatformIDs(0, ಖಾಲಿ, &ಅಂಕೆ_Platforms);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ಆದರೆ (ಅಂಕೆ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ಅಂಕೆ_Platforms);
    ಸ್ಥಿತಿ_2 = clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)ಅಂಕೆ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು = -1;
        cl_device_id device = ಖಾಲಿ;
        cl_uint ಅಂಕೆ_Devices = 0;
        cl_context context = ಖಾಲಿ;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        ಆದರೆ (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            ಮುಂದುವರಿ;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ಮಾಡು {
            device = ಖಾಲಿ;
            ಅಂಕೆ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ಅಂಕೆ_Devices);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                ನಿಲ್ಲಿಸು;
            ಆದರೆ (ಅಂಕೆ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ಖಾಲಿ, ಖಾಲಿ
                };
                context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಸಿಕ್ಕಿತು = i;
                }
            }
        } ಆಗ (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ಆದರೆ (ಸಿಕ್ಕಿತು < 0) ಮಾಡು
        {
            device = ಖಾಲಿ;
            ಅಂಕೆ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ಅಂಕೆ_Devices);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                ನಿಲ್ಲಿಸು;
            ಆದರೆ (ಅಂಕೆ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ಖಾಲಿ, ಖಾಲಿ
                };
                context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಸಿಕ್ಕಿತು = i;
                }
            }
        } ಆಗ (0);

        ಆದರೆ (ಸಿಕ್ಕಿತು >= 0) {
            OpenCLExecutionContext clExecCtx;
            ಪ್ರಯತ್ನಿಸು
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ಗಣ_User_Context(std::make_shared<OpenCL_D3D9>(platform, ಶೂನ್ಯಸೂಚಕ, pDirect3DDevice9Ex));
            }
            ಹಿಡಿ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ಎಸೆ;
            }
            clExecCtx.bind();
            ಹಿಂತಿರುಗಿ const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ಆದರೆ_ಅಂತ್ಯ
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_ಬಳಸದ(pDirect3DDevice9);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ
    cl_uint ಅಂಕೆ_Platforms;
    cl_int ಸ್ಥಿತಿ_2 = clGetPlatformIDs(0, ಖಾಲಿ, &ಅಂಕೆ_Platforms);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    ಆದರೆ (ಅಂಕೆ_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(ಅಂಕೆ_Platforms);
    ಸ್ಥಿತಿ_2 = clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)ಅಂಕೆ_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು = -1;
        cl_device_id device = ಖಾಲಿ;
        cl_uint ಅಂಕೆ_Devices = 0;
        cl_context context = ಖಾಲಿ;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        ಆದರೆ (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            ಮುಂದುವರಿ;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ಮಾಡು {
            device = ಖಾಲಿ;
            ಅಂಕೆ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ಅಂಕೆ_Devices);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                ನಿಲ್ಲಿಸು;
            ಆದರೆ (ಅಂಕೆ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ಖಾಲಿ, ಖಾಲಿ
                };
                context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಸಿಕ್ಕಿತು = i;
                    ನಿಲ್ಲಿಸು;
                }
            }
        } ಆಗ (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        ಆದರೆ (ಸಿಕ್ಕಿತು < 0) ಮಾಡು
        {
            device = ಖಾಲಿ;
            ಅಂಕೆ_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &ಅಂಕೆ_Devices);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                ನಿಲ್ಲಿಸು;
            ಆದರೆ (ಅಂಕೆ_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        ಖಾಲಿ, ಖಾಲಿ
                };
                context = clCreateContext(properties, 1, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
                ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ಇಲ್ಲದಿದ್ದರೆ
                {
                    ಸಿಕ್ಕಿತು = i;
                    ನಿಲ್ಲಿಸು;
                }
            }
        } ಆಗ (0);

        ಆದರೆ (ಸಿಕ್ಕಿತು >= 0) {
            OpenCLExecutionContext clExecCtx;
            ಪ್ರಯತ್ನಿಸು
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ಗಣ_User_Context(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, ಶೂನ್ಯಸೂಚಕ));
            }
            ಹಿಡಿ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ಎಸೆ;
            }
            clExecCtx.bind();
            ಹಿಂತಿರುಗಿ const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#ಆದರೆ_ಅಂತ್ಯ
}

} // namespace cv::ocl

} // namespace directx


ನಾಮಾಕಾಶ ocl {

#ಆದರೆ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12

ಸ್ಥಿರ
ತಾರ್ಕಿಕ ocl_convert_nv12_ಗಮ್ಯ_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem cl_ಬಫರ್,
    ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ,
    ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ,
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(clImageY, clImageUV, cl_ಬಫರ್, ಹೆಜ್ಜೆ, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು);

    size_t globalsize[] = { (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/2, (size_t)ಅಡ್ಡಸಾಲುಗಳು/2 };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, 0, ಸುಳ್ಳು);
}


ಸ್ಥಿರ
ತಾರ್ಕಿಕ ocl_convert_bgr_ಗಮ್ಯ_nv12(
    cl_mem cl_ಬಫರ್,
    ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ,
    ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ,
    ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(cl_ಬಫರ್, ಹೆಜ್ಜೆ, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/2, (size_t)ಅಡ್ಡಸಾಲುಗಳು/2 };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, 0, ಸುಳ್ಳು);
}

#ಆದರೆ_ಅಂತ್ಯ // HAVE_DIRECTX_NV12
#ಆದರೆ_ಅಂತ್ಯ // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


ನಾಮಾಕಾಶ directx {

#ಆದರೆ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ಸ್ಥಿರ ಶೂನ್ಯ __convertToD3D11Texture2DKHR(ಒಳಹರಿವು_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ಪೂರ್ಣಾಂಕ srcType = src.type();
    ಪೂರ್ಣಾಂಕ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ಗಾತ್ರ_2 src_ಗಾತ್ರ = src.size();
    CV_Assert(src_ಗಾತ್ರ.ಅಗಲ == (ಪೂರ್ಣಾಂಕ)desc.ಅಗಲ_2 && src_ಗಾತ್ರ.ಎತ್ತರ == (ಪೂರ್ಣಾಂಕ)desc.ಎತ್ತರ_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#ಆದರೆ_ಅಂತ್ಯ
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#ಆದರೆ_ಅಂತ್ಯ

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        ಆದರೆ(!ocl::ocl_convert_bgr_ಗಮ್ಯ_nv12(cl_ಬಫರ್, (ಪೂರ್ಣಾಂಕ)u.ಹೆಜ್ಜೆ[0], u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, u.ಅಡ್ಡಸಾಲುಗಳು, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1 };

        ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_ಗಮ್ಯ_Image(q, cl_ಬಫರ್, clImage, offset, origin, region, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageUV);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(HAVE_OPENCL_D3D11_NV)
ಸ್ಥಿರ ಶೂನ್ಯ __convertToD3D11Texture2DNV(ಒಳಹರಿವು_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ಪೂರ್ಣಾಂಕ srcType = src.type();
    ಪೂರ್ಣಾಂಕ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ಗಾತ್ರ_2 src_ಗಾತ್ರ = src.size();
    CV_Assert(src_ಗಾತ್ರ.ಅಗಲ == (ಪೂರ್ಣಾಂಕ)desc.ಅಗಲ_2 && src_ಗಾತ್ರ.ಎತ್ತರ == (ಪೂರ್ಣಾಂಕ)desc.ಎತ್ತರ_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = 0;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#ಆದರೆ_ಅಂತ್ಯ
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#ಆದರೆ_ಅಂತ್ಯ
    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        ಆದರೆ(!ocl::ocl_convert_bgr_ಗಮ್ಯ_nv12(cl_ಬಫರ್, (ಪೂರ್ಣಾಂಕ)u.ಹೆಜ್ಜೆ[0], u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, u.ಅಡ್ಡಸಾಲುಗಳು, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1 };

        ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_ಗಮ್ಯ_Image(q, cl_ಬಫರ್, clImage, offset, origin, region, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageUV);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ಸ್ಥಿರ ಶೂನ್ಯ __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ಪೂರ್ಣಾಂಕ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ಗಾತ್ರ_2(desc.ಅಗಲ_2, desc.ಎತ್ತರ_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#ಆದರೆ_ಅಂತ್ಯ

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();

    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        ಆದರೆ(!ocl::ocl_convert_nv12_ಗಮ್ಯ_bgr(clImage, clImageUV, cl_ಬಫರ್, (ಪೂರ್ಣಾಂಕ)u.ಹೆಜ್ಜೆ[0], u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, u.ಅಡ್ಡಸಾಲುಗಳು))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1 };

        ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_Image_ಗಮ್ಯ_ಬಫರ್(q, clImage, cl_ಬಫರ್, origin, region, offset, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageUV);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(HAVE_OPENCL_D3D11_NV)
ಸ್ಥಿರ ಶೂನ್ಯ __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    ಪೂರ್ಣಾಂಕ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ಗಾತ್ರ_2(desc.ಅಗಲ_2, desc.ಎತ್ತರ_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#ಆದರೆ_ಅಂತ್ಯ

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        ಆದರೆ (!ocl::ocl_convert_nv12_ಗಮ್ಯ_bgr(clImage, clImageUV, cl_ಬಫರ್, (ಪೂರ್ಣಾಂಕ)u.ಹೆಜ್ಜೆ[0], u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, u.ಅಡ್ಡಸಾಲುಗಳು))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1 };

        ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_Image_ಗಮ್ಯ_ಬಫರ್(q, clImage, cl_ಬಫರ್, origin, region, offset, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_DIRECTX_NV12
    ಆದರೆ(DXGI_FORMAT_NV12 == desc.Format)
    {
        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageUV);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#ಆದರೆ_ಅಂತ್ಯ
}
#ಆದರೆ_ಅಂತ್ಯ

ಶೂನ್ಯ convert_ಗಮ್ಯ_D3_D11_Texture2_D(ಒಳಹರಿವು_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(pD3D11Texture2D);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ಆದರೆ (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ಆದರೆ (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_ಬಳಸದ(pD3D11Texture2D); CV_ಬಳಸದ(dst);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    ಆದರೆ (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        ಹಿಂತಿರುಗಿ;
    }
#ಆದರೆ_ಅಂತ್ಯ
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    ಆದರೆ (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    ಇಲ್ಲದಿದ್ದರೆ {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ convert_ಗಮ್ಯ_D3_D10_Texture2_D(ಒಳಹರಿವು_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(pD3D10Texture2D);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    ಪೂರ್ಣಾಂಕ srcType = src.type();
    ಪೂರ್ಣಾಂಕ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ಗಾತ್ರ_2 src_ಗಾತ್ರ = src.size();
    CV_Assert(src_ಗಾತ್ರ.ಅಗಲ == (ಪೂರ್ಣಾಂಕ)desc.ಅಗಲ_2 && src_ಗಾತ್ರ.ಎತ್ತರ == (ಪೂರ್ಣಾಂಕ)desc.ಎತ್ತರ_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1};
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_ಗಮ್ಯ_Image(q, cl_ಬಫರ್, clImage, offset, dst_origin, region, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ಇಲ್ಲದಿದ್ದರೆ
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_ಬಳಸದ(pD3D10Texture2D); CV_ಬಳಸದ(dst);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    ಪೂರ್ಣಾಂಕ textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ಗಾತ್ರ_2(desc.ಅಗಲ_2, desc.ಎತ್ತರ_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D10Texture2D, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1};
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_Image_ಗಮ್ಯ_ಬಫರ್(q, clImage, cl_ಬಫರ್, src_origin, region, offset, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ಇಲ್ಲದಿದ್ದರೆ
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ convert_ಗಮ್ಯ_Direct3_DSurface9(ಒಳಹರಿವು_Array src, IDirect3DSurface9* pDirect3DSurface9, ಶೂನ್ಯ* surfaceSharedHandle)
{
    CV_ಬಳಸದ(src); CV_ಬಳಸದ(pDirect3DSurface9); CV_ಬಳಸದ(surfaceSharedHandle);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    ಆದರೆ (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    ಪೂರ್ಣಾಂಕ srcType = src.type();
    ಪೂರ್ಣಾಂಕ surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    ಗಾತ್ರ_2 src_ಗಾತ್ರ = src.size();
    CV_Assert(src_ಗಾತ್ರ.ಅಗಲ == (ಪೂರ್ಣಾಂಕ)desc.ಅಗಲ_2 && src_ಗಾತ್ರ.ಎತ್ತರ == (ಪೂರ್ಣಾಂಕ)desc.ಎತ್ತರ_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_WRITE_ONLY,
        impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1};
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_ಬಫರ್_ಗಮ್ಯ_Image(q, cl_ಬಫರ್, clImage, offset, dst_origin, region, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ಇಲ್ಲದಿದ್ದರೆ
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#ಆದರೆ_ಅಂತ್ಯ
}

ಶೂನ್ಯ convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, ಶೂನ್ಯ* surfaceSharedHandle)
{
    CV_ಬಳಸದ(pDirect3DSurface9); CV_ಬಳಸದ(dst); CV_ಬಳಸದ(surfaceSharedHandle);
#ಆದರೆ !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ಅಥವಾಆದರೆ defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    ಆದರೆ (ಶೂನ್ಯಸೂಚಕ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    ಆದರೆ (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    ಪೂರ್ಣಾಂಕ surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ಗಾತ್ರ_2(desc.ಅಗಲ_2, desc.ಎತ್ತರ_2), surfaceType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int ಸ್ಥಿತಿ_2 = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_READ_ONLY,
            impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &ಸ್ಥಿತಿ_2);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ಸರತಿ::getDefault().ptr();
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, (size_t)u.ಅಡ್ಡಸಾಲುಗಳು, 1};
    ಸ್ಥಿತಿ_2 = cl_ಸರತಿಗೆ_ಸೇರಿಸು_Copy_Image_ಗಮ್ಯ_ಬಫರ್(q, clImage, cl_ಬಫರ್, src_origin, region, offset, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    ಸ್ಥಿತಿ_2 = impl->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, ಖಾಲಿ, ಖಾಲಿ);
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImage); // TODO RAII
    ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ಇಲ್ಲದಿದ್ದರೆ
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#ಆದರೆ_ಅಂತ್ಯ
}

} } // namespace cv::directx
