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

#ضمن "precomp.hpp"

#ضمن "opencv2/core.hpp"
#ضمن "opencv2/core/ocl.hpp"
#ضمن "opencv2/core/directx.hpp"
#ضمن "opencl_kernels_core.hpp"

#جے_تعریف_شدہ HAVE_DIRECTX
#ضمن <vector>
#ضمن "directx.inc.hpp"
#وإلا // HAVE_DIRECTX
#تعریف_کرو NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#جے_انت

#جے_بنا_تعریف HAVE_OPENCL
#تعریف_کرو NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#جے_انت // HAVE_OPENCL

ورتوں نطاق cv::ocl;

نطاق cv { نطاق directx {

صحيح getTypeFromDXGI_FORMAT(ثابت صحيح iDXGI_FORMAT)
{
    CV_بن_ورتیا(iDXGI_FORMAT);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلا
    ثابت صحيح errorType = -1;
    بدل ((تعداد DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    حالة DXGI_FORMAT_R32G32B32A32_FLOAT: أرجع CV_32FC4;
    حالة DXGI_FORMAT_R32G32B32A32_UINT:
    حالة DXGI_FORMAT_R32G32B32A32_SINT:  أرجع CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    حالة DXGI_FORMAT_R32G32B32_FLOAT: أرجع CV_32FC3;
    حالة DXGI_FORMAT_R32G32B32_UINT:
    حالة DXGI_FORMAT_R32G32B32_SINT: أرجع CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    حالة DXGI_FORMAT_R16G16B16A16_FLOAT: أرجع CV_16FC4;
    حالة DXGI_FORMAT_R16G16B16A16_UNORM:
    حالة DXGI_FORMAT_R16G16B16A16_UINT: أرجع CV_16UC4;
    حالة DXGI_FORMAT_R16G16B16A16_SNORM:
    حالة DXGI_FORMAT_R16G16B16A16_SINT: أرجع CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    حالة DXGI_FORMAT_R32G32_FLOAT: أرجع CV_32FC2;
    حالة DXGI_FORMAT_R32G32_UINT:
    حالة DXGI_FORMAT_R32G32_SINT: أرجع CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    حالة DXGI_FORMAT_R8G8B8A8_UNORM:
    حالة DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    حالة DXGI_FORMAT_R8G8B8A8_UINT: أرجع CV_8UC4;
    حالة DXGI_FORMAT_R8G8B8A8_SNORM:
    حالة DXGI_FORMAT_R8G8B8A8_SINT: أرجع CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    حالة DXGI_FORMAT_R16G16_FLOAT: أرجع CV_16FC2;
    حالة DXGI_FORMAT_R16G16_UNORM:
    حالة DXGI_FORMAT_R16G16_UINT: أرجع CV_16UC2;
    حالة DXGI_FORMAT_R16G16_SNORM:
    حالة DXGI_FORMAT_R16G16_SINT: أرجع CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    حالة DXGI_FORMAT_D32_FLOAT:
    حالة DXGI_FORMAT_R32_FLOAT: أرجع CV_32FC1;
    حالة DXGI_FORMAT_R32_UINT:
    حالة DXGI_FORMAT_R32_SINT: أرجع CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    حالة DXGI_FORMAT_R8G8_UNORM:
    حالة DXGI_FORMAT_R8G8_UINT: أرجع CV_8UC2;
    حالة DXGI_FORMAT_R8G8_SNORM:
    حالة DXGI_FORMAT_R8G8_SINT: أرجع CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    حالة DXGI_FORMAT_R16_FLOAT: أرجع CV_16FC1;
    حالة DXGI_FORMAT_D16_UNORM:
    حالة DXGI_FORMAT_R16_UNORM:
    حالة DXGI_FORMAT_R16_UINT: أرجع CV_16UC1;
    حالة DXGI_FORMAT_R16_SNORM:
    حالة DXGI_FORMAT_R16_SINT: أرجع CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    حالة DXGI_FORMAT_R8_UNORM:
    حالة DXGI_FORMAT_R8_UINT: أرجع CV_8UC1;
    حالة DXGI_FORMAT_R8_SNORM:
    حالة DXGI_FORMAT_R8_SINT: أرجع CV_8SC1;
    حالة DXGI_FORMAT_A8_UNORM: أرجع CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    حالة DXGI_FORMAT_R8G8_B8G8_UNORM:
    حالة DXGI_FORMAT_G8R8_G8B8_UNORM: أرجع CV_8UC4;
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
    حالة DXGI_FORMAT_B8G8R8A8_UNORM:
    حالة DXGI_FORMAT_B8G8R8X8_UNORM: أرجع CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    حالة DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: أرجع CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    حالة DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: أرجع CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#جے_تعریف_شدہ HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    حالة DXGI_FORMAT_NV12: أرجع CV_8UC3;
#جے_انت
    افتراضي: اكسر;
    }
    أرجع errorType;
#جے_انت
}

صحيح getTypeFromD3DFORMAT(ثابت صحيح iD3DFORMAT)
{
    CV_بن_ورتیا(iD3DFORMAT);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلا
    ثابت صحيح errorType = -1;
    بدل ((تعداد _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    حالة D3DFMT_R8G8B8: أرجع CV_8UC3;
    حالة D3DFMT_A8R8G8B8:
    حالة D3DFMT_X8R8G8B8: أرجع CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    حالة D3DFMT_A8: أرجع CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    حالة D3DFMT_A8B8G8R8:
    حالة D3DFMT_X8B8G8R8: أرجع CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    حالة D3DFMT_A8P8: أرجع CV_8UC2;
    حالة D3DFMT_P8: أرجع CV_8UC1;

    حالة D3DFMT_L8: أرجع CV_8UC1;
    حالة D3DFMT_A8L8: أرجع CV_8UC2;
    //case D3DFMT_A4L4:

    حالة D3DFMT_V8U8: أرجع CV_8UC2;
    //case D3DFMT_L6V5U5:
    حالة D3DFMT_X8L8V8U8:
    حالة D3DFMT_Q8W8V8U8: أرجع CV_8UC4;
    حالة D3DFMT_V16U16: أرجع CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    حالة D3DFMT_D16_LOCKABLE: أرجع CV_16UC1;
    حالة D3DFMT_D32: أرجع CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    حالة D3DFMT_D16: أرجع CV_16UC1;

    حالة D3DFMT_D32F_LOCKABLE: أرجع CV_32FC1;
    افتراضي: اكسر;
    }
    أرجع errorType;
#جے_انت
}

#إذا defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#جے_تعریف_شدہ HAVE_OPENCL_D3D11_NV
صنف OpenCL_D3D11_NV : عام ocl::Context::UserContext
{
عام:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromD3D11Texture2DNV");
        cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV = (cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV = (cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        إذا (!clCreateFromD3D11Texture2DNV || !cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV || !cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11_NV");
        }
        device->جوڑو_Ref();
    }
    ~OpenCL_D3D11_NV() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DNV_fn clCreateFromD3D11Texture2DNV;
    cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV_fn cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV;
    cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV_fn cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV;
};
#جے_انت

صنف OpenCL_D3D11 : عام ocl::Context::UserContext
{
عام:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromD3D11Texture2DKHR");
        cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR = (cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR = (cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        إذا (!clCreateFromD3D11Texture2DKHR || !cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR || !cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D11");
        }
        device->جوڑو_Ref();
    }
    ~OpenCL_D3D11() {
        device->Release();
    }
    ID3D11Device* device;
    clCreateFromD3D11Texture2DKHR_fn clCreateFromD3D11Texture2DKHR;
    cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR_fn cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR;
    cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR_fn cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR;
};

صنف OpenCL_D3D9 : عام ocl::Context::UserContext
{
عام:
    OpenCL_D3D9(cl_platform_id platform, IDirect3DDevice9* _device, IDirect3DDevice9Ex* _deviceEx)
        : device(_device)
        , deviceEx(_deviceEx)
    {
        clCreateFromDX9MediaSurfaceKHR = (clCreateFromDX9MediaSurfaceKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromDX9MediaSurfaceKHR");
        cl_قطار_وچ_پاؤ_Acquire_DX9_Media_Surfaces_KHR = (cl_قطار_وچ_پاؤ_Acquire_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireDX9MediaSurfacesKHR");
        cl_قطار_وچ_پاؤ_Release_DX9_Media_Surfaces_KHR = (cl_قطار_وچ_پاؤ_Release_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseDX9MediaSurfacesKHR");
        إذا (!clCreateFromDX9MediaSurfaceKHR || !cl_قطار_وچ_پاؤ_Acquire_DX9_Media_Surfaces_KHR || !cl_قطار_وچ_پاؤ_Release_DX9_Media_Surfaces_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        إذا (device)
            device->جوڑو_Ref();
        إذا (deviceEx)
            deviceEx->جوڑو_Ref();
    }
    ~OpenCL_D3D9() {
        إذا (device)
            device->Release();
        إذا (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    cl_قطار_وچ_پاؤ_Acquire_DX9_Media_Surfaces_KHR_fn cl_قطار_وچ_پاؤ_Acquire_DX9_Media_Surfaces_KHR;
    cl_قطار_وچ_پاؤ_Release_DX9_Media_Surfaces_KHR_fn cl_قطار_وچ_پاؤ_Release_DX9_Media_Surfaces_KHR;
};

صنف OpenCL_D3D10 : عام ocl::Context::UserContext
{
عام:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromD3D10Texture2DKHR");
        cl_قطار_وچ_پاؤ_Acquire_D3_D10_Objects_KHR = (cl_قطار_وچ_پاؤ_Acquire_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        cl_قطار_وچ_پاؤ_Release_D3_D10_Objects_KHR = (cl_قطار_وچ_پاؤ_Release_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        إذا (!clCreateFromD3D10Texture2DKHR || !cl_قطار_وچ_پاؤ_Acquire_D3_D10_Objects_KHR || !cl_قطار_وچ_پاؤ_Release_D3_D10_Objects_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D10");
        }
        device->جوڑو_Ref();
    }
    ~OpenCL_D3D10() {
        device->Release();
    }
    ID3D10Device* device;
    clCreateFromD3D10Texture2DKHR_fn clCreateFromD3D10Texture2DKHR;
    cl_قطار_وچ_پاؤ_Acquire_D3_D10_Objects_KHR_fn cl_قطار_وچ_پاؤ_Acquire_D3_D10_Objects_KHR;
    cl_قطار_وچ_پاؤ_Release_D3_D10_Objects_KHR_fn cl_قطار_وچ_پاؤ_Release_D3_D10_Objects_KHR;
};
#جے_انت

نطاق ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_بن_ورتیا(pD3D11Device);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلا
    cl_uint عدد_Platforms;
    cl_int حالت = clGetPlatformIDs(0, عدم, &عدد_Platforms);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    إذا (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت = clGetPlatformIDs(عدد_Platforms, &platforms[0], عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    لكل (صحيح i = 0; i < (صحيح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        صحيح لبھیا = -1;
        cl_device_id device = عدم;
        cl_uint عدد_Devices = 0;
        cl_context context = عدم;

#جے_تعریف_شدہ HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        إذا (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            افعل {
                device = عدم;
                عدد_Devices = 0;
                حالت = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &عدد_Devices);
                إذا (حالت != CL_SUCCESS)
                    اكسر;
                إذا (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                    إذا (حالت != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    وإلا
                    {
                        لبھیا = i;
                    }
                }
            } طالما (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            إذا (لبھیا < 0) افعل {
                device = عدم;
                عدد_Devices = 0;
                حالت = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &عدد_Devices);
                إذا (حالت != CL_SUCCESS)
                    اكسر;
                إذا (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                    إذا (حالت != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    وإلا
                    {
                        لبھیا = i;
                    }
                }
            } طالما (0);
            إذا (لبھیا >= 0) {
                OpenCLExecutionContext clExecCtx;
                حاول
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                امسك (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    ارم;
                }
                clExecCtx.bind();
                أرجع const_cast<Context&>(clExecCtx.getContext());
            }
        }
#جے_انت
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        إذا (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            افعل {

                device = عدم;
                عدد_Devices = 0;

                حالت = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &عدد_Devices);

                إذا (حالت != CL_SUCCESS)
                    اكسر;
                إذا (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            عدم, عدم
                    };
                    context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                    إذا (حالت != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    وإلا
                    {
                        لبھیا = i;
                    }
                }
            } طالما (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            إذا (لبھیا < 0) افعل {
                device = عدم;
                عدد_Devices = 0;
                حالت = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &عدد_Devices);
                إذا (حالت != CL_SUCCESS)
                    اكسر;
                إذا (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            عدم, عدم
                    };
                    context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                    إذا (حالت != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    وإلا
                    {
                        لبھیا = i;
                    }
                }
            } طالما (0);

            إذا (لبھیا >= 0) {
                OpenCLExecutionContext clExecCtx;
                حاول
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                امسك (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    ارم;
                }
                clExecCtx.bind();
                أرجع const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#جے_انت
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_بن_ورتیا(pD3D10Device);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلا
    cl_uint عدد_Platforms;
    cl_int حالت = clGetPlatformIDs(0, عدم, &عدد_Platforms);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    إذا (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت = clGetPlatformIDs(عدد_Platforms, &platforms[0], عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    لكل (صحيح i = 0; i < (صحيح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        صحيح لبھیا = -1;
        cl_device_id device = عدم;
        cl_uint عدد_Devices = 0;
        cl_context context = عدم;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        إذا (!clGetDeviceIDsFromD3D10KHR)
            استمر;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        افعل {
            device = عدم;
            عدد_Devices = 0;
            حالت = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &عدد_Devices);
            إذا (حالت != CL_SUCCESS)
                اكسر;
            إذا (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        عدم, عدم
                };
                context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                إذا (حالت != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                وإلا
                {
                    لبھیا = i;
                }
            }
        } طالما (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        إذا (لبھیا < 0) افعل
        {
            device = عدم;
            عدد_Devices = 0;
            حالت = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &عدد_Devices);
            إذا (حالت != CL_SUCCESS)
                اكسر;
            إذا (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        عدم, عدم
                };
                context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                إذا (حالت != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                وإلا
                {
                    لبھیا = i;
                }
            }
        } طالما (0);

        إذا (لبھیا >= 0) {
            OpenCLExecutionContext clExecCtx;
            حاول
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            امسك (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ارم;
            }
            clExecCtx.bind();
            أرجع const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#جے_انت
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_بن_ورتیا(pDirect3DDevice9Ex);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلا
    cl_uint عدد_Platforms;
    cl_int حالت = clGetPlatformIDs(0, عدم, &عدد_Platforms);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    إذا (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت = clGetPlatformIDs(عدد_Platforms, &platforms[0], عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    لكل (صحيح i = 0; i < (صحيح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        صحيح لبھیا = -1;
        cl_device_id device = عدم;
        cl_uint عدد_Devices = 0;
        cl_context context = عدم;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        إذا (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            استمر;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        افعل {
            device = عدم;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            حالت = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            إذا (حالت != CL_SUCCESS)
                اكسر;
            إذا (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        عدم, عدم
                };
                context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                إذا (حالت != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                وإلا
                {
                    لبھیا = i;
                }
            }
        } طالما (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        إذا (لبھیا < 0) افعل
        {
            device = عدم;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            حالت = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            إذا (حالت != CL_SUCCESS)
                اكسر;
            إذا (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        عدم, عدم
                };
                context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                إذا (حالت != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                وإلا
                {
                    لبھیا = i;
                }
            }
        } طالما (0);

        إذا (لبھیا >= 0) {
            OpenCLExecutionContext clExecCtx;
            حاول
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D9>(platform, خالی_سوچک, pDirect3DDevice9Ex));
            }
            امسك (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ارم;
            }
            clExecCtx.bind();
            أرجع const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#جے_انت
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_بن_ورتیا(pDirect3DDevice9);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلا
    cl_uint عدد_Platforms;
    cl_int حالت = clGetPlatformIDs(0, عدم, &عدد_Platforms);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    إذا (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت = clGetPlatformIDs(عدد_Platforms, &platforms[0], عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    لكل (صحيح i = 0; i < (صحيح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        صحيح لبھیا = -1;
        cl_device_id device = عدم;
        cl_uint عدد_Devices = 0;
        cl_context context = عدم;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        إذا (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            استمر;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        افعل {
            device = عدم;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            حالت = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            إذا (حالت != CL_SUCCESS)
                اكسر;
            إذا (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        عدم, عدم
                };
                context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                إذا (حالت != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                وإلا
                {
                    لبھیا = i;
                    اكسر;
                }
            }
        } طالما (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        إذا (لبھیا < 0) افعل
        {
            device = عدم;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            حالت = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            إذا (حالت != CL_SUCCESS)
                اكسر;
            إذا (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        عدم, عدم
                };
                context = clCreateContext(properties, 1, &device, عدم, عدم, &حالت);
                إذا (حالت != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                وإلا
                {
                    لبھیا = i;
                    اكسر;
                }
            }
        } طالما (0);

        إذا (لبھیا >= 0) {
            OpenCLExecutionContext clExecCtx;
            حاول
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, خالی_سوچک));
            }
            امسك (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ارم;
            }
            clExecCtx.bind();
            أرجع const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#جے_انت
}

} // namespace cv::ocl

} // namespace directx


نطاق ocl {

#إذا defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#جے_تعریف_شدہ HAVE_DIRECTX_NV12

ساكن
منطقي ocl_convert_nv12_منزل_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem cl_بفر,
    صحيح قدم,
    صحيح خانے_گنتی,
    صحيح صفاں)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلاں(clImageY, clImageUV, cl_بفر, قدم, خانے_گنتی, صفاں);

    size_t globalsize[] = { (size_t)خانے_گنتی/2, (size_t)صفاں/2 };
    أرجع k.لگاتار(2, globalsize, 0, خطأ);
}


ساكن
منطقي ocl_convert_bgr_منزل_nv12(
    cl_mem cl_بفر,
    صحيح قدم,
    صحيح خانے_گنتی,
    صحيح صفاں,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلاں(cl_بفر, قدم, خانے_گنتی, صفاں, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)خانے_گنتی/2, (size_t)صفاں/2 };
    أرجع k.لگاتار(2, globalsize, 0, خطأ);
}

#جے_انت // HAVE_DIRECTX_NV12
#جے_انت // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


نطاق directx {

#إذا defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ساكن فراغ __convertToD3D11Texture2DKHR(داخلہ_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحيح srcType = src.type();
    صحيح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ناپ src_ناپ = src.size();
    CV_Assert(src_ناپ.چوڑائی == (صحيح)desc.چوڑائی_2 && src_ناپ.اچائی == (صحيح)desc.اچائی_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت = 0;
    cl_mem clImage = 0;
#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#جے_انت
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#جے_انت

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        إذا(!ocl::ocl_convert_bgr_منزل_nv12(cl_بفر, (صحيح)u.قدم[0], u.خانے_گنتی, u.صفاں, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    وإلا
#جے_انت
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.خانے_گنتی, (size_t)u.صفاں, 1 };

        حالت = cl_قطار_وچ_پاؤ_Copy_بفر_منزل_Image(q, cl_بفر, clImage, offset, origin, region, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = clReleaseMemObject(clImageUV);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#جے_انت
}
#جے_انت

#إذا defined(HAVE_OPENCL_D3D11_NV)
ساكن فراغ __convertToD3D11Texture2DNV(داخلہ_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحيح srcType = src.type();
    صحيح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ناپ src_ناپ = src.size();
    CV_Assert(src_ناپ.چوڑائی == (صحيح)desc.چوڑائی_2 && src_ناپ.اچائی == (صحيح)desc.اچائی_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت = 0;
    cl_mem clImage = 0;
#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#جے_انت
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#جے_انت
    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        إذا(!ocl::ocl_convert_bgr_منزل_nv12(cl_بفر, (صحيح)u.قدم[0], u.خانے_گنتی, u.صفاں, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    وإلا
#جے_انت
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.خانے_گنتی, (size_t)u.صفاں, 1 };

        حالت = cl_قطار_وچ_پاؤ_Copy_بفر_منزل_Image(q, cl_بفر, clImage, offset, origin, region, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = clReleaseMemObject(clImageUV);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#جے_انت
}
#جے_انت

#إذا defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ساكن فراغ __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحيح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ناپ(desc.چوڑائی_2, desc.اچائی_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#جے_انت

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        إذا(!ocl::ocl_convert_nv12_منزل_bgr(clImage, clImageUV, cl_بفر, (صحيح)u.قدم[0], u.خانے_گنتی, u.صفاں))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    وإلا
#جے_انت
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.خانے_گنتی, (size_t)u.صفاں, 1 };

        حالت = cl_قطار_وچ_پاؤ_Copy_Image_منزل_بفر(q, clImage, cl_بفر, origin, region, offset, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = clReleaseMemObject(clImageUV);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#جے_انت
}
#جے_انت

#إذا defined(HAVE_OPENCL_D3D11_NV)
ساكن فراغ __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحيح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ناپ(desc.چوڑائی_2, desc.اچائی_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#جے_انت

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        إذا (!ocl::ocl_convert_nv12_منزل_bgr(clImage, clImageUV, cl_بفر, (صحيح)u.قدم[0], u.خانے_گنتی, u.صفاں))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    وإلا
#جے_انت
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.خانے_گنتی, (size_t)u.صفاں, 1 };

        حالت = cl_قطار_وچ_پاؤ_Copy_Image_منزل_بفر(q, clImage, cl_بفر, origin, region, offset, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#جے_تعریف_شدہ HAVE_DIRECTX_NV12
    إذا(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت = clReleaseMemObject(clImageUV);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#جے_انت
}
#جے_انت

فراغ convert_منزل_D3_D11_Texture2_D(داخلہ_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_بن_ورتیا(src); CV_بن_ورتیا(pD3D11Texture2D);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلا

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#جے_تعریف_شدہ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    إذا (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        أرجع;
    }
#جے_انت
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    إذا (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    وإلا {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#جے_انت
}

فراغ convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_بن_ورتیا(pD3D11Texture2D); CV_بن_ورتیا(dst);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#وإلا

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#جے_تعریف_شدہ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    إذا (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        أرجع;
    }
#جے_انت
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    إذا (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    وإلا {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#جے_انت
}

فراغ convert_منزل_D3_D10_Texture2_D(داخلہ_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_بن_ورتیا(src); CV_بن_ورتیا(pD3D10Texture2D);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    صحيح srcType = src.type();
    صحيح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    ناپ src_ناپ = src.size();
    CV_Assert(src_ناپ.چوڑائی == (صحيح)desc.چوڑائی_2 && src_ناپ.اچائی == (صحيح)desc.اچائی_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.خانے_گنتی, (size_t)u.صفاں, 1};
    حالت = cl_قطار_وچ_پاؤ_Copy_بفر_منزل_Image(q, cl_بفر, clImage, offset, dst_origin, region, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#وإلا
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#جے_انت
}

فراغ convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_بن_ورتیا(pD3D10Texture2D); CV_بن_ورتیا(dst);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    صحيح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ناپ(desc.چوڑائی_2, desc.اچائی_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D10Texture2D, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.خانے_گنتی, (size_t)u.صفاں, 1};
    حالت = cl_قطار_وچ_پاؤ_Copy_Image_منزل_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    حالت = impl->cl_قطار_وچ_پاؤ_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#وإلا
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#جے_انت
}

فراغ convert_منزل_Direct3_DSurface9(داخلہ_Array src, IDirect3DSurface9* pDirect3DSurface9, فراغ* surfaceSharedHandle)
{
    CV_بن_ورتیا(src); CV_بن_ورتیا(pDirect3DSurface9); CV_بن_ورتیا(surfaceSharedHandle);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    إذا (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    صحيح srcType = src.type();
    صحيح surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    ناپ src_ناپ = src.size();
    CV_Assert(src_ناپ.چوڑائی == (صحيح)desc.چوڑائی_2 && src_ناپ.اچائی == (صحيح)desc.اچائی_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_WRITE_ONLY,
        impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.خانے_گنتی, (size_t)u.صفاں, 1};
    حالت = cl_قطار_وچ_پاؤ_Copy_بفر_منزل_Image(q, cl_بفر, clImage, offset, dst_origin, region, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    حالت = impl->cl_قطار_وچ_پاؤ_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#وإلا
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#جے_انت
}

فراغ convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, فراغ* surfaceSharedHandle)
{
    CV_بن_ورتیا(pDirect3DSurface9); CV_بن_ورتیا(dst); CV_بن_ورتیا(surfaceSharedHandle);
#إذا !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وإلاإذا defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    إذا (خالی_سوچک == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    إذا (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    صحيح surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(ناپ(desc.چوڑائی_2, desc.اچائی_2), surfaceType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_READ_ONLY,
            impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &حالت);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت = impl->cl_قطار_وچ_پاؤ_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.خانے_گنتی, (size_t)u.صفاں, 1};
    حالت = cl_قطار_وچ_پاؤ_Copy_Image_منزل_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    حالت = impl->cl_قطار_وچ_پاؤ_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, عدم, عدم);
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    حالت = cl_انت(q); // TODO Use events
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت = clReleaseMemObject(clImage); // TODO RAII
    إذا (حالت != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#وإلا
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#جے_انت
}

} } // namespace cv::directx
