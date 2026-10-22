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

#شامل "precomp.hpp"

#شامل "opencv2/core.hpp"
#شامل "opencv2/core/ocl.hpp"
#شامل "opencv2/core/directx.hpp"
#شامل "opencl_kernels_core.hpp"

#اگر_تعریف_شدہ HAVE_DIRECTX
#شامل <vector>
#شامل "directx.inc.hpp"
#ورنہ // HAVE_DIRECTX
#تعریف_کرو NO_DIRECTX_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without DirectX support")
#اگر_ختم

#اگر_غیر_تعریف_شدہ HAVE_OPENCL
#تعریف_کرو NO_OPENCL_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without OpenCL support")
#اگر_ختم // HAVE_OPENCL

استعمال نامخلا cv::ocl;

نامخلا cv { نامخلا directx {

صحیح getTypeFromDXGI_FORMAT(ثابت صحیح iDXGI_FORMAT)
{
    CV_غیر_مستعمل(iDXGI_FORMAT);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ورنہ
    ثابت صحیح errorType = -1;
    انتخاب ((شمار DXGI_FORMAT)iDXGI_FORMAT)
    {
    //case DXGI_FORMAT_UNKNOWN:
    //case DXGI_FORMAT_R32G32B32A32_TYPELESS:
    حالت DXGI_FORMAT_R32G32B32A32_FLOAT: واپس CV_32FC4;
    حالت DXGI_FORMAT_R32G32B32A32_UINT:
    حالت DXGI_FORMAT_R32G32B32A32_SINT:  واپس CV_32SC4;
    //case DXGI_FORMAT_R32G32B32_TYPELESS:
    حالت DXGI_FORMAT_R32G32B32_FLOAT: واپس CV_32FC3;
    حالت DXGI_FORMAT_R32G32B32_UINT:
    حالت DXGI_FORMAT_R32G32B32_SINT: واپس CV_32SC3;
    //case DXGI_FORMAT_R16G16B16A16_TYPELESS:
    حالت DXGI_FORMAT_R16G16B16A16_FLOAT: واپس CV_16FC4;
    حالت DXGI_FORMAT_R16G16B16A16_UNORM:
    حالت DXGI_FORMAT_R16G16B16A16_UINT: واپس CV_16UC4;
    حالت DXGI_FORMAT_R16G16B16A16_SNORM:
    حالت DXGI_FORMAT_R16G16B16A16_SINT: واپس CV_16SC4;
    //case DXGI_FORMAT_R32G32_TYPELESS:
    حالت DXGI_FORMAT_R32G32_FLOAT: واپس CV_32FC2;
    حالت DXGI_FORMAT_R32G32_UINT:
    حالت DXGI_FORMAT_R32G32_SINT: واپس CV_32SC2;
    //case DXGI_FORMAT_R32G8X24_TYPELESS:
    //case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
    //case DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS:
    //case DXGI_FORMAT_X32_TYPELESS_G8X24_UINT:
    //case DXGI_FORMAT_R10G10B10A2_TYPELESS:
    //case DXGI_FORMAT_R10G10B10A2_UNORM:
    //case DXGI_FORMAT_R10G10B10A2_UINT:
    //case DXGI_FORMAT_R11G11B10_FLOAT:
    //case DXGI_FORMAT_R8G8B8A8_TYPELESS:
    حالت DXGI_FORMAT_R8G8B8A8_UNORM:
    حالت DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
    حالت DXGI_FORMAT_R8G8B8A8_UINT: واپس CV_8UC4;
    حالت DXGI_FORMAT_R8G8B8A8_SNORM:
    حالت DXGI_FORMAT_R8G8B8A8_SINT: واپس CV_8SC4;
    //case DXGI_FORMAT_R16G16_TYPELESS:
    حالت DXGI_FORMAT_R16G16_FLOAT: واپس CV_16FC2;
    حالت DXGI_FORMAT_R16G16_UNORM:
    حالت DXGI_FORMAT_R16G16_UINT: واپس CV_16UC2;
    حالت DXGI_FORMAT_R16G16_SNORM:
    حالت DXGI_FORMAT_R16G16_SINT: واپس CV_16SC2;
    //case DXGI_FORMAT_R32_TYPELESS:
    حالت DXGI_FORMAT_D32_FLOAT:
    حالت DXGI_FORMAT_R32_FLOAT: واپس CV_32FC1;
    حالت DXGI_FORMAT_R32_UINT:
    حالت DXGI_FORMAT_R32_SINT: واپس CV_32SC1;
    //case DXGI_FORMAT_R24G8_TYPELESS:
    //case DXGI_FORMAT_D24_UNORM_S8_UINT:
    //case DXGI_FORMAT_R24_UNORM_X8_TYPELESS:
    //case DXGI_FORMAT_X24_TYPELESS_G8_UINT:
    //case DXGI_FORMAT_R8G8_TYPELESS:
    حالت DXGI_FORMAT_R8G8_UNORM:
    حالت DXGI_FORMAT_R8G8_UINT: واپس CV_8UC2;
    حالت DXGI_FORMAT_R8G8_SNORM:
    حالت DXGI_FORMAT_R8G8_SINT: واپس CV_8SC2;
    //case DXGI_FORMAT_R16_TYPELESS:
    حالت DXGI_FORMAT_R16_FLOAT: واپس CV_16FC1;
    حالت DXGI_FORMAT_D16_UNORM:
    حالت DXGI_FORMAT_R16_UNORM:
    حالت DXGI_FORMAT_R16_UINT: واپس CV_16UC1;
    حالت DXGI_FORMAT_R16_SNORM:
    حالت DXGI_FORMAT_R16_SINT: واپس CV_16SC1;
    //case DXGI_FORMAT_R8_TYPELESS:
    حالت DXGI_FORMAT_R8_UNORM:
    حالت DXGI_FORMAT_R8_UINT: واپس CV_8UC1;
    حالت DXGI_FORMAT_R8_SNORM:
    حالت DXGI_FORMAT_R8_SINT: واپس CV_8SC1;
    حالت DXGI_FORMAT_A8_UNORM: واپس CV_8UC1;
    //case DXGI_FORMAT_R1_UNORM:
    //case DXGI_FORMAT_R9G9B9E5_SHAREDEXP:
    حالت DXGI_FORMAT_R8G8_B8G8_UNORM:
    حالت DXGI_FORMAT_G8R8_G8B8_UNORM: واپس CV_8UC4;
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
    حالت DXGI_FORMAT_B8G8R8A8_UNORM:
    حالت DXGI_FORMAT_B8G8R8X8_UNORM: واپس CV_8UC4;
    //case DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM:
    //case DXGI_FORMAT_B8G8R8A8_TYPELESS:
    حالت DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: واپس CV_8UC4;
    //case DXGI_FORMAT_B8G8R8X8_TYPELESS:
    حالت DXGI_FORMAT_B8G8R8X8_UNORM_SRGB: واپس CV_8UC4;
    //case DXGI_FORMAT_BC6H_TYPELESS:
    //case DXGI_FORMAT_BC6H_UF16:
    //case DXGI_FORMAT_BC6H_SF16:
    //case DXGI_FORMAT_BC7_TYPELESS:
    //case DXGI_FORMAT_BC7_UNORM:
    //case DXGI_FORMAT_BC7_UNORM_SRGB:
#اگر_تعریف_شدہ HAVE_DIRECTX_NV12 //D3DX11 should support DXGI_FORMAT_NV12.
    حالت DXGI_FORMAT_NV12: واپس CV_8UC3;
#اگر_ختم
    طےشدہ: توڑو;
    }
    واپس errorType;
#اگر_ختم
}

صحیح getTypeFromD3DFORMAT(ثابت صحیح iD3DFORMAT)
{
    CV_غیر_مستعمل(iD3DFORMAT);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#ورنہ
    ثابت صحیح errorType = -1;
    انتخاب ((شمار _D3DFORMAT)iD3DFORMAT)
    {
    //case D3DFMT_UNKNOWN:
    حالت D3DFMT_R8G8B8: واپس CV_8UC3;
    حالت D3DFMT_A8R8G8B8:
    حالت D3DFMT_X8R8G8B8: واپس CV_8UC4;
    //case D3DFMT_R5G6B5:
    //case D3DFMT_X1R5G5B5:
    //case D3DFMT_A1R5G5B5:
    //case D3DFMT_A4R4G4B4:
    //case D3DFMT_R3G3B2:
    حالت D3DFMT_A8: واپس CV_8UC1;
    //case D3DFMT_A8R3G3B2:
    //case D3DFMT_X4R4G4B4:
    //case D3DFMT_A2B10G10R10:
    حالت D3DFMT_A8B8G8R8:
    حالت D3DFMT_X8B8G8R8: واپس CV_8UC4;
    //case D3DFMT_G16R16:
    //case D3DFMT_A2R10G10B10:
    //case D3DFMT_A16B16G16R16:

    حالت D3DFMT_A8P8: واپس CV_8UC2;
    حالت D3DFMT_P8: واپس CV_8UC1;

    حالت D3DFMT_L8: واپس CV_8UC1;
    حالت D3DFMT_A8L8: واپس CV_8UC2;
    //case D3DFMT_A4L4:

    حالت D3DFMT_V8U8: واپس CV_8UC2;
    //case D3DFMT_L6V5U5:
    حالت D3DFMT_X8L8V8U8:
    حالت D3DFMT_Q8W8V8U8: واپس CV_8UC4;
    حالت D3DFMT_V16U16: واپس CV_16UC4; // TODO 16SC4 ?
    //case D3DFMT_A2W10V10U10:

    حالت D3DFMT_D16_LOCKABLE: واپس CV_16UC1;
    حالت D3DFMT_D32: واپس CV_32SC1;
    //case D3DFMT_D15S1:
    //case D3DFMT_D24S8:
    //case D3DFMT_D24X8:
    //case D3DFMT_D24X4S4:
    حالت D3DFMT_D16: واپس CV_16UC1;

    حالت D3DFMT_D32F_LOCKABLE: واپس CV_32FC1;
    طےشدہ: توڑو;
    }
    واپس errorType;
#اگر_ختم
}

#اگر defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)

#اگر_تعریف_شدہ HAVE_OPENCL_D3D11_NV
جماعت OpenCL_D3D11_NV : عوامی ocl::Context::UserContext
{
عوامی:
    OpenCL_D3D11_NV(cl_platform_id platform, ID3D11Device*_device) : device(_device)
    {
        clCreateFromD3D11Texture2DNV = (clCreateFromD3D11Texture2DNV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromD3D11Texture2DNV");
        cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV = (cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsNV");
        cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV = (cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsNV");
        اگر (!clCreateFromD3D11Texture2DNV || !cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV || !cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV)
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
    cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV_fn cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV;
    cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV_fn cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV;
};
#اگر_ختم

جماعت OpenCL_D3D11 : عوامی ocl::Context::UserContext
{
عوامی:
    OpenCL_D3D11(cl_platform_id platform, ID3D11Device* _device) : device(_device)
    {
        clCreateFromD3D11Texture2DKHR = (clCreateFromD3D11Texture2DKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromD3D11Texture2DKHR");
        cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR = (cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireD3D11ObjectsKHR");
        cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR = (cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseD3D11ObjectsKHR");
        اگر (!clCreateFromD3D11Texture2DKHR || !cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR || !cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR)
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
    cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR_fn cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR;
    cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR_fn cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR;
};

جماعت OpenCL_D3D9 : عوامی ocl::Context::UserContext
{
عوامی:
    OpenCL_D3D9(cl_platform_id platform, IDirect3DDevice9* _device, IDirect3DDevice9Ex* _deviceEx)
        : device(_device)
        , deviceEx(_deviceEx)
    {
        clCreateFromDX9MediaSurfaceKHR = (clCreateFromDX9MediaSurfaceKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromDX9MediaSurfaceKHR");
        cl_قطار_میں_ڈالو_Acquire_DX9_Media_Surfaces_KHR = (cl_قطار_میں_ڈالو_Acquire_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireDX9MediaSurfacesKHR");
        cl_قطار_میں_ڈالو_Release_DX9_Media_Surfaces_KHR = (cl_قطار_میں_ڈالو_Release_DX9_Media_Surfaces_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseDX9MediaSurfacesKHR");
        اگر (!clCreateFromDX9MediaSurfaceKHR || !cl_قطار_میں_ڈالو_Acquire_DX9_Media_Surfaces_KHR || !cl_قطار_میں_ڈالو_Release_DX9_Media_Surfaces_KHR)
        {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't find functions for D3D9");
        }
        اگر (device)
            device->جوڑو_Ref();
        اگر (deviceEx)
            deviceEx->جوڑو_Ref();
    }
    ~OpenCL_D3D9() {
        اگر (device)
            device->Release();
        اگر (deviceEx)
            deviceEx->Release();
    }
    IDirect3DDevice9* device;
    IDirect3DDevice9Ex* deviceEx;
    clCreateFromDX9MediaSurfaceKHR_fn clCreateFromDX9MediaSurfaceKHR;
    cl_قطار_میں_ڈالو_Acquire_DX9_Media_Surfaces_KHR_fn cl_قطار_میں_ڈالو_Acquire_DX9_Media_Surfaces_KHR;
    cl_قطار_میں_ڈالو_Release_DX9_Media_Surfaces_KHR_fn cl_قطار_میں_ڈالو_Release_DX9_Media_Surfaces_KHR;
};

جماعت OpenCL_D3D10 : عوامی ocl::Context::UserContext
{
عوامی:
    OpenCL_D3D10(cl_platform_id platform, ID3D10Device* _device) : device(_device)
    {
        clCreateFromD3D10Texture2DKHR = (clCreateFromD3D10Texture2DKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromD3D10Texture2DKHR");
        cl_قطار_میں_ڈالو_Acquire_D3_D10_Objects_KHR = (cl_قطار_میں_ڈالو_Acquire_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireD3D10ObjectsKHR");
        cl_قطار_میں_ڈالو_Release_D3_D10_Objects_KHR = (cl_قطار_میں_ڈالو_Release_D3_D10_Objects_KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseD3D10ObjectsKHR");
        اگر (!clCreateFromD3D10Texture2DKHR || !cl_قطار_میں_ڈالو_Acquire_D3_D10_Objects_KHR || !cl_قطار_میں_ڈالو_Release_D3_D10_Objects_KHR)
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
    cl_قطار_میں_ڈالو_Acquire_D3_D10_Objects_KHR_fn cl_قطار_میں_ڈالو_Acquire_D3_D10_Objects_KHR;
    cl_قطار_میں_ڈالو_Release_D3_D10_Objects_KHR_fn cl_قطار_میں_ڈالو_Release_D3_D10_Objects_KHR;
};
#اگر_ختم

نامخلا ocl {

Context& initializeContextFromD3D11Device(ID3D11Device* pD3D11Device)
{
    CV_غیر_مستعمل(pD3D11Device);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ورنہ
    cl_uint عدد_Platforms;
    cl_int حالت_2 = clGetPlatformIDs(0, تہی, &عدد_Platforms);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    اگر (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت_2 = clGetPlatformIDs(عدد_Platforms, &platforms[0], تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE

    برائے (صحیح i = 0; i < (صحیح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();

        صحیح ملا = -1;
        cl_device_id device = تہی;
        cl_uint عدد_Devices = 0;
        cl_context context = تہی;

#اگر_تعریف_شدہ HAVE_OPENCL_D3D11_NV
        // Get extension function "clGetDeviceIDsFromD3D11NV" (part of OpenCL extension "cl_nv_d3d11_sharing")
        clGetDeviceIDsFromD3D11NV_fn clGetDeviceIDsFromD3D11NV = (clGetDeviceIDsFromD3D11NV_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11NV");
        اگر (clGetDeviceIDsFromD3D11NV) {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_NV
            کرو {
                device = تہی;
                عدد_Devices = 0;
                حالت_2 = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_NV, 1, &device, &عدد_Devices);
                اگر (حالت_2 != CL_SUCCESS)
                    توڑو;
                اگر (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };

                    context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                    اگر (حالت_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ورنہ
                    {
                        ملا = i;
                    }
                }
            } جب (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_NV
            اگر (ملا < 0) کرو {
                device = تہی;
                عدد_Devices = 0;
                حالت_2 = clGetDeviceIDsFromD3D11NV(platforms[i], CL_D3D11_DEVICE_NV, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_NV, 1, &device, &عدد_Devices);
                اگر (حالت_2 != CL_SUCCESS)
                    توڑو;
                اگر (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_NV, (cl_context_properties)(pD3D11Device),
                            //CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            0
                    };
                    context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                    اگر (حالت_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ورنہ
                    {
                        ملا = i;
                    }
                }
            } جب (0);
            اگر (ملا >= 0) {
                OpenCLExecutionContext clExecCtx;
                کوشش
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D11_NV>(platform, pD3D11Device));
                }
                پکڑو (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    پھینکو;
                }
                clExecCtx.bind();
                واپس const_cast<Context&>(clExecCtx.getContext());
            }
        }
#اگر_ختم
        // Get extension function "clGetDeviceIDsFromD3D11KHR" (part of OpenCL extension "cl_khr_d3d11_sharing")
        clGetDeviceIDsFromD3D11KHR_fn clGetDeviceIDsFromD3D11KHR = (clGetDeviceIDsFromD3D11KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D11KHR");
        اگر (clGetDeviceIDsFromD3D11KHR)
        {
            // try with CL_PREFERRED_DEVICES_FOR_D3D11_KHR
            کرو {

                device = تہی;
                عدد_Devices = 0;

                حالت_2 = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_PREFERRED_DEVICES_FOR_D3D11_KHR, 1, &device, &عدد_Devices);

                اگر (حالت_2 != CL_SUCCESS)
                    توڑو;
                اگر (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            تہی, تہی
                    };
                    context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                    اگر (حالت_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ورنہ
                    {
                        ملا = i;
                    }
                }
            } جب (0);
            // try with CL_ALL_DEVICES_FOR_D3D11_KHR
            اگر (ملا < 0) کرو {
                device = تہی;
                عدد_Devices = 0;
                حالت_2 = clGetDeviceIDsFromD3D11KHR(platforms[i], CL_D3D11_DEVICE_KHR, pD3D11Device,
                    CL_ALL_DEVICES_FOR_D3D11_KHR, 1, &device, &عدد_Devices);
                اگر (حالت_2 != CL_SUCCESS)
                    توڑو;
                اگر (عدد_Devices > 0)
                {
                    cl_context_properties properties[] = {
                            CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                            CL_CONTEXT_D3D11_DEVICE_KHR, (cl_context_properties)(pD3D11Device),
                            CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                            تہی, تہی
                    };
                    context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                    اگر (حالت_2 != CL_SUCCESS)
                    {
                        clReleaseDevice(device);
                    }
                    ورنہ
                    {
                        ملا = i;
                    }
                }
            } جب (0);

            اگر (ملا >= 0) {
                OpenCLExecutionContext clExecCtx;
                کوشش
                {
                    clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                    clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D11>(platform, pD3D11Device));
                }
                پکڑو (...)
                {
                    clReleaseDevice(device);
                    clReleaseContext(context);
                    پھینکو;
                }
                clExecCtx.bind();
                واپس const_cast<Context&>(clExecCtx.getContext());
            }
        }
    }

    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#اگر_ختم
}

Context& initializeContextFromD3D10Device(ID3D10Device* pD3D10Device)
{
    CV_غیر_مستعمل(pD3D10Device);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ورنہ
    cl_uint عدد_Platforms;
    cl_int حالت_2 = clGetPlatformIDs(0, تہی, &عدد_Platforms);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    اگر (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت_2 = clGetPlatformIDs(عدد_Platforms, &platforms[0], تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    برائے (صحیح i = 0; i < (صحیح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        صحیح ملا = -1;
        cl_device_id device = تہی;
        cl_uint عدد_Devices = 0;
        cl_context context = تہی;

        clGetDeviceIDsFromD3D10KHR_fn clGetDeviceIDsFromD3D10KHR = (clGetDeviceIDsFromD3D10KHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromD3D10KHR");
        اگر (!clGetDeviceIDsFromD3D10KHR)
            جاری;

        // try with CL_PREFERRED_DEVICES_FOR_D3D10_KHR
        کرو {
            device = تہی;
            عدد_Devices = 0;
            حالت_2 = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_PREFERRED_DEVICES_FOR_D3D10_KHR, 1, &device, &عدد_Devices);
            اگر (حالت_2 != CL_SUCCESS)
                توڑو;
            اگر (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        تہی, تہی
                };
                context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                اگر (حالت_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ورنہ
                {
                    ملا = i;
                }
            }
        } جب (0);
        // try with CL_ALL_DEVICES_FOR_D3D10_KHR
        اگر (ملا < 0) کرو
        {
            device = تہی;
            عدد_Devices = 0;
            حالت_2 = clGetDeviceIDsFromD3D10KHR(platforms[i], CL_D3D10_DEVICE_KHR, pD3D10Device,
                CL_ALL_DEVICES_FOR_D3D10_KHR, 1, &device, &عدد_Devices);
            اگر (حالت_2 != CL_SUCCESS)
                توڑو;
            اگر (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_D3D10_DEVICE_KHR, (cl_context_properties)(pD3D10Device),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        تہی, تہی
                };
                context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                اگر (حالت_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ورنہ
                {
                    ملا = i;
                }
            }
        } جب (0);

        اگر (ملا >= 0) {
            OpenCLExecutionContext clExecCtx;
            کوشش
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D10>(platform, pD3D10Device));
            }
            پکڑو (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                پھینکو;
            }
            clExecCtx.bind();
            واپس const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#اگر_ختم
}

Context& initializeContextFromDirect3DDevice9Ex(IDirect3DDevice9Ex* pDirect3DDevice9Ex)
{
    CV_غیر_مستعمل(pDirect3DDevice9Ex);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ورنہ
    cl_uint عدد_Platforms;
    cl_int حالت_2 = clGetPlatformIDs(0, تہی, &عدد_Platforms);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    اگر (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت_2 = clGetPlatformIDs(عدد_Platforms, &platforms[0], تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    برائے (صحیح i = 0; i < (صحیح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        صحیح ملا = -1;
        cl_device_id device = تہی;
        cl_uint عدد_Devices = 0;
        cl_context context = تہی;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        اگر (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            جاری;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        کرو {
            device = تہی;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            حالت_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            اگر (حالت_2 != CL_SUCCESS)
                توڑو;
            اگر (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        تہی, تہی
                };
                context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                اگر (حالت_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ورنہ
                {
                    ملا = i;
                }
            }
        } جب (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        اگر (ملا < 0) کرو
        {
            device = تہی;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9EX_KHR;
            حالت_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9Ex,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            اگر (حالت_2 != CL_SUCCESS)
                توڑو;
            اگر (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9EX_KHR, (cl_context_properties)(pDirect3DDevice9Ex),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        تہی, تہی
                };
                context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                اگر (حالت_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ورنہ
                {
                    ملا = i;
                }
            }
        } جب (0);

        اگر (ملا >= 0) {
            OpenCLExecutionContext clExecCtx;
            کوشش
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D9>(platform, خالی_اشاریہ, pDirect3DDevice9Ex));
            }
            پکڑو (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                پھینکو;
            }
            clExecCtx.bind();
            واپس const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#اگر_ختم
}

Context& initializeContextFromDirect3DDevice9(IDirect3DDevice9* pDirect3DDevice9)
{
    CV_غیر_مستعمل(pDirect3DDevice9);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ورنہ
    cl_uint عدد_Platforms;
    cl_int حالت_2 = clGetPlatformIDs(0, تہی, &عدد_Platforms);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
    اگر (عدد_Platforms == 0)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

    std::vector<cl_platform_id> platforms(عدد_Platforms);
    حالت_2 = clGetPlatformIDs(عدد_Platforms, &platforms[0], تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platforms");

    // TODO Filter platforms by name from OPENCV_OPENCL_DEVICE
    برائے (صحیح i = 0; i < (صحیح)عدد_Platforms; i++)
    {
        cl_platform_id platform = platforms[i];
        std::string platformName = PlatformInfo(&platform).name();
        صحیح ملا = -1;
        cl_device_id device = تہی;
        cl_uint عدد_Devices = 0;
        cl_context context = تہی;

        clGetDeviceIDsFromDX9MediaAdapterKHR_fn clGetDeviceIDsFromDX9MediaAdapterKHR = (clGetDeviceIDsFromDX9MediaAdapterKHR_fn)
            cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromDX9MediaAdapterKHR");
        اگر (!clGetDeviceIDsFromDX9MediaAdapterKHR)
            جاری;

        // try with CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        کرو {
            device = تہی;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            حالت_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_PREFERRED_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            اگر (حالت_2 != CL_SUCCESS)
                توڑو;
            اگر (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        تہی, تہی
                };
                context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                اگر (حالت_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ورنہ
                {
                    ملا = i;
                    توڑو;
                }
            }
        } جب (0);
        // try with CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR
        اگر (ملا < 0) کرو
        {
            device = تہی;
            عدد_Devices = 0;
            cl_dx9_media_adapter_type_khr type = CL_ADAPTER_D3D9_KHR;
            حالت_2 = clGetDeviceIDsFromDX9MediaAdapterKHR(platforms[i], 1, &type, &pDirect3DDevice9,
                CL_ALL_DEVICES_FOR_DX9_MEDIA_ADAPTER_KHR, 1, &device, &عدد_Devices);
            اگر (حالت_2 != CL_SUCCESS)
                توڑو;
            اگر (عدد_Devices > 0)
            {
                cl_context_properties properties[] = {
                        CL_CONTEXT_PLATFORM, (cl_context_properties)platforms[i],
                        CL_CONTEXT_ADAPTER_D3D9_KHR, (cl_context_properties)(pDirect3DDevice9),
                        CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE,
                        تہی, تہی
                };
                context = clCreateContext(properties, 1, &device, تہی, تہی, &حالت_2);
                اگر (حالت_2 != CL_SUCCESS)
                {
                    clReleaseDevice(device);
                }
                ورنہ
                {
                    ملا = i;
                    توڑو;
                }
            }
        } جب (0);

        اگر (ملا >= 0) {
            OpenCLExecutionContext clExecCtx;
            کوشش
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<OpenCL_D3D9>(platform, pDirect3DDevice9, خالی_اشاریہ));
            }
            پکڑو (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                پھینکو;
            }
            clExecCtx.bind();
            واپس const_cast<Context&>(clExecCtx.getContext());
        }
    }
    CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't create context for DirectX interop");
#اگر_ختم
}

} // namespace cv::ocl

} // namespace directx


نامخلا ocl {

#اگر defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
#اگر_تعریف_شدہ HAVE_DIRECTX_NV12

ساکن
منطقی ocl_convert_nv12_مقصد_bgr(
    cl_mem clImageY,
    cl_mem clImageUV,
    cl_mem cl_بفر,
    صحیح قدم,
    صحیح ستون_تعداد,
    صحیح صفیں)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(clImageY, clImageUV, cl_بفر, قدم, ستون_تعداد, صفیں);

    size_t globalsize[] = { (size_t)ستون_تعداد/2, (size_t)صفیں/2 };
    واپس k.تسلسل(2, globalsize, 0, جھوٹ);
}


ساکن
منطقی ocl_convert_bgr_مقصد_nv12(
    cl_mem cl_بفر,
    صحیح قدم,
    صحیح ستون_تعداد,
    صحیح صفیں,
    cl_mem clImageY,
    cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(cl_بفر, قدم, ستون_تعداد, صفیں, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)ستون_تعداد/2, (size_t)صفیں/2 };
    واپس k.تسلسل(2, globalsize, 0, جھوٹ);
}

#اگر_ختم // HAVE_DIRECTX_NV12
#اگر_ختم // HAVE_DIRECTX && HAVE_OPENCL

} // namespace ocl


نامخلا directx {

#اگر defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ساکن خالی __convertToD3D11Texture2DKHR(داخلہ_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحیح srcType = src.type();
    صحیح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    سائز src_سائز = src.size();
    CV_Assert(src_سائز.چوڑائی == (صحیح)desc.چوڑائی_2 && src_سائز.اونچائی == (صحیح)desc.اونچائی_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت_2 = 0;
    cl_mem clImage = 0;
#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#اگر_ختم
    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#اگر_ختم

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        اگر(!ocl::ocl_convert_bgr_مقصد_nv12(cl_بفر, (صحیح)u.قدم[0], u.ستون_تعداد, u.صفیں, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    ورنہ
#اگر_ختم
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ستون_تعداد, (size_t)u.صفیں, 1 };

        حالت_2 = cl_قطار_میں_ڈالو_Copy_بفر_مقصد_Image(q, cl_بفر, clImage, offset, origin, region, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = clReleaseMemObject(clImageUV);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#اگر_ختم
}
#اگر_ختم

#اگر defined(HAVE_OPENCL_D3D11_NV)
ساکن خالی __convertToD3D11Texture2DNV(داخلہ_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحیح srcType = src.type();
    صحیح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    سائز src_سائز = src.size();
    CV_Assert(src_سائز.چوڑائی == (صحیح)desc.چوڑائی_2 && src_سائز.اونچائی == (صحیح)desc.اونچائی_2);

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت_2 = 0;
    cl_mem clImage = 0;
#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
#اگر_ختم
    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر (DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_WRITE_ONLY, pD3D11Texture2D, 1, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#اگر_ختم
    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        اگر(!ocl::ocl_convert_bgr_مقصد_nv12(cl_بفر, (صحیح)u.قدم[0], u.ستون_تعداد, u.صفیں, clImage, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");

        حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    ورنہ
#اگر_ختم
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ستون_تعداد, (size_t)u.صفیں, 1 };

        حالت_2 = cl_قطار_میں_ڈالو_Copy_بفر_مقصد_Image(q, cl_بفر, clImage, offset, origin, region, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    }

    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = clReleaseMemObject(clImageUV);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#اگر_ختم
}
#اگر_ختم

#اگر defined(HAVE_DIRECTX) && defined(HAVE_OPENCL)
ساکن خالی __convertFromD3D11Texture2DKHR(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحیح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(سائز(desc.چوڑائی_2, desc.اونچائی_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت_2 = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DKHR failed");
    }
#اگر_ختم

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();

    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsKHR failed");

        اگر(!ocl::ocl_convert_nv12_مقصد_bgr(clImage, clImageUV, cl_بفر, (صحیح)u.قدم[0], u.ستون_تعداد, u.صفیں))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");
    }
    ورنہ
#اگر_ختم
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ستون_تعداد, (size_t)u.صفیں, 1 };

        حالت_2 = cl_قطار_میں_ڈالو_Copy_Image_مقصد_بفر(q, clImage, cl_بفر, origin, region, offset, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsKHR failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = clReleaseMemObject(clImageUV);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#اگر_ختم
}
#اگر_ختم

#اگر defined(HAVE_OPENCL_D3D11_NV)
ساکن خالی __convertFromD3D11Texture2DNV(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    D3D11_TEXTURE2D_DESC desc = { 0 };
    pD3D11Texture2D->GetDesc(&desc);

    صحیح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(سائز(desc.چوڑائی_2, desc.اونچائی_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    cl_context context = (cl_context)ctx.ptr();
    OpenCL_D3D11_NV* impl = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    cl_int حالت_2 = 0;
    cl_mem clImage = 0;

    clImage = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    cl_mem clImageUV = 0;
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        clImageUV = impl->clCreateFromD3D11Texture2DNV(context, CL_MEM_READ_ONLY, pD3D11Texture2D, 1, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D11Texture2DNV failed");
    }
#اگر_ختم

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر (DXGI_FORMAT::DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D11_Objects_NV(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D11ObjectsNV failed");

        اگر (!ocl::ocl_convert_nv12_مقصد_bgr(clImage, clImageUV, cl_بفر, (صحیح)u.قدم[0], u.ستون_تعداد, u.صفیں))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");

        حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV(q, 1, &clImageUV, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");
    }
    ورنہ
#اگر_ختم
    {
        size_t offset = 0; // TODO
        size_t origin[3] = { 0, 0, 0 };
        size_t region[3] = { (size_t)u.ستون_تعداد, (size_t)u.صفیں, 1 };

        حالت_2 = cl_قطار_میں_ڈالو_Copy_Image_مقصد_بفر(q, clImage, cl_بفر, origin, region, offset, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    }

    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D11_Objects_NV(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D11ObjectsNV failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");

#اگر_تعریف_شدہ HAVE_DIRECTX_NV12
    اگر(DXGI_FORMAT_NV12 == desc.Format)
    {
        حالت_2 = clReleaseMemObject(clImageUV);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
    }
#اگر_ختم
}
#اگر_ختم

خالی convert_مقصد_D3_D11_Texture2_D(داخلہ_Array src, ID3D11Texture2D* pD3D11Texture2D)
{
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(pD3D11Texture2D);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ورنہ

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#اگر_تعریف_شدہ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    اگر (impl_nv) {
        __convertToD3D11Texture2DNV(src,pD3D11Texture2D);
        واپس;
    }
#اگر_ختم
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    اگر (impl) {
        __convertToD3D11Texture2DKHR(src, pD3D11Texture2D);
    }
    ورنہ {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#اگر_ختم
}

خالی convertFromD3D11Texture2D(ID3D11Texture2D* pD3D11Texture2D, OutputArray dst)
{
    CV_غیر_مستعمل(pD3D11Texture2D); CV_غیر_مستعمل(dst);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر !defined(HAVE_OPENCL)
    NO_OPENCL_SUPPORT_ERROR;
#ورنہ

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
#اگر_تعریف_شدہ HAVE_OPENCL_D3D11_NV
    OpenCL_D3D11_NV* impl_nv = ctx.getUserContext<OpenCL_D3D11_NV>().get();
    اگر (impl_nv) {
        __convertFromD3D11Texture2DNV(pD3D11Texture2D,dst);
        واپس;
    }
#اگر_ختم
    OpenCL_D3D11* impl = ctx.getUserContext<OpenCL_D3D11>().get();
    اگر (impl) {
        __convertFromD3D11Texture2DKHR(pD3D11Texture2D, dst);
    }
    ورنہ {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");
    }
#اگر_ختم
}

خالی convert_مقصد_D3_D10_Texture2_D(داخلہ_Array src, ID3D10Texture2D* pD3D10Texture2D)
{
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(pD3D10Texture2D);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    صحیح srcType = src.type();
    صحیح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType == srcType);

    سائز src_سائز = src.size();
    CV_Assert(src_سائز.چوڑائی == (صحیح)desc.چوڑائی_2 && src_سائز.اونچائی == (صحیح)desc.اونچائی_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت_2 = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_WRITE_ONLY, pD3D10Texture2D, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ستون_تعداد, (size_t)u.صفیں, 1};
    حالت_2 = cl_قطار_میں_ڈالو_Copy_بفر_مقصد_Image(q, cl_بفر, clImage, offset, dst_origin, region, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ورنہ
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#اگر_ختم
}

خالی convertFromD3D10Texture2D(ID3D10Texture2D* pD3D10Texture2D, OutputArray dst)
{
    CV_غیر_مستعمل(pD3D10Texture2D); CV_غیر_مستعمل(dst);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر defined(HAVE_OPENCL)
    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D10* impl = ctx.getUserContext<OpenCL_D3D10>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3D10_TEXTURE2D_DESC desc = { 0 };
    pD3D10Texture2D->GetDesc(&desc);

    صحیح textureType = getTypeFromDXGI_FORMAT(desc.Format);
    CV_Assert(textureType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(سائز(desc.چوڑائی_2, desc.اونچائی_2), textureType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت_2 = 0;
    cl_mem clImage = impl->clCreateFromD3D10Texture2DKHR(context, CL_MEM_READ_ONLY, pD3D10Texture2D, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromD3D10Texture2DKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_D3_D10_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireD3D10ObjectsKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ستون_تعداد, (size_t)u.صفیں, 1};
    حالت_2 = cl_قطار_میں_ڈالو_Copy_Image_مقصد_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_D3_D10_Objects_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseD3D10ObjectsKHR failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ورنہ
    // TODO memcpy
    NO_OPENCL_SUPPORT_ERROR;
#اگر_ختم
}

خالی convert_مقصد_Direct3_DSurface9(داخلہ_Array src, IDirect3DSurface9* pDirect3DSurface9, خالی* surfaceSharedHandle)
{
    CV_غیر_مستعمل(src); CV_غیر_مستعمل(pDirect3DSurface9); CV_غیر_مستعمل(surfaceSharedHandle);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    اگر (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    صحیح srcType = src.type();
    صحیح surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType == srcType);

    سائز src_سائز = src.size();
    CV_Assert(src_سائز.چوڑائی == (صحیح)desc.چوڑائی_2 && src_سائز.اونچائی == (صحیح)desc.اونچائی_2);

    cl_context context = (cl_context)ctx.ptr();

    UMat u = src.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت_2 = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_WRITE_ONLY,
        impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t dst_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ستون_تعداد, (size_t)u.صفیں, 1};
    حالت_2 = cl_قطار_میں_ڈالو_Copy_بفر_مقصد_Image(q, cl_بفر, clImage, offset, dst_origin, region, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyBufferToImage failed");
    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ورنہ
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#اگر_ختم
}

خالی convertFromDirect3DSurface9(IDirect3DSurface9* pDirect3DSurface9, OutputArray dst, خالی* surfaceSharedHandle)
{
    CV_غیر_مستعمل(pDirect3DSurface9); CV_غیر_مستعمل(dst); CV_غیر_مستعمل(surfaceSharedHandle);
#اگر !defined(HAVE_DIRECTX)
    NO_DIRECTX_SUPPORT_ERROR;
#وگرنہ_اگر defined(HAVE_OPENCL)

    ocl::Context& ctx = ocl::OpenCLExecutionContext::getCurrent().getContext();
    OpenCL_D3D9* impl = ctx.getUserContext<OpenCL_D3D9>().get();
    اگر (خالی_اشاریہ == impl)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Context initilized without DirectX interoperability");

    D3DSURFACE_DESC desc;
    اگر (FAILED(pDirect3DSurface9->GetDesc(&desc)))
    {
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: Can't get D3D surface description");
    }

    صحیح surfaceType = getTypeFromD3DFORMAT(desc.Format);
    CV_Assert(surfaceType >= 0);

    cl_context context = (cl_context)ctx.ptr();

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(سائز(desc.چوڑائی_2, desc.اونچائی_2), surfaceType);
    UMat u = dst.getUMat();

    // TODO Add support for roi
    CV_Assert(u.offset == 0);
    CV_Assert(u.isContinuous());

    cl_int حالت_2 = 0;
    cl_dx9_surface_info_khr surfaceInfo = {pDirect3DSurface9, (HANDLE)surfaceSharedHandle};
    cl_mem clImage = impl->clCreateFromDX9MediaSurfaceKHR(context, CL_MEM_READ_ONLY,
            impl->deviceEx ? CL_ADAPTER_D3D9EX_KHR : CL_ADAPTER_D3D9_KHR,
            &surfaceInfo, 0, &حالت_2);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromDX9MediaSurfaceKHR failed");

    cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_WRITE);

    cl_command_قطار q = (cl_command_قطار)قطار::getDefault().ptr();
    حالت_2 = impl->cl_قطار_میں_ڈالو_Acquire_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireDX9MediaSurfacesKHR failed");
    size_t offset = 0; // TODO
    size_t src_origin[3] = {0, 0, 0};
    size_t region[3] = {(size_t)u.ستون_تعداد, (size_t)u.صفیں, 1};
    حالت_2 = cl_قطار_میں_ڈالو_Copy_Image_مقصد_بفر(q, clImage, cl_بفر, src_origin, region, offset, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueCopyImageToBuffer failed");
    حالت_2 = impl->cl_قطار_میں_ڈالو_Release_DX9_Media_Surfaces_KHR(q, 1, &clImage, 0, تہی, تہی);
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseDX9MediaSurfacesKHR failed");

    حالت_2 = cl_اختتام(q); // TODO Use events
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

    حالت_2 = clReleaseMemObject(clImage); // TODO RAII
    اگر (حالت_2 != CL_SUCCESS)
        CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed");
#ورنہ
    // TODO pDirect3DSurface9->LockRect() + memcpy + Unlock()
    NO_OPENCL_SUPPORT_ERROR;
#اگر_ختم
}

} } // namespace cv::directx
