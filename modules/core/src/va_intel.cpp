// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ਅਨ੍ਤਰ੍ਭਾਵ "precomp.hpp"

#ਅਨ੍ਤਰ੍ਭਾਵ <opencv2/core/utils/logger.hpp>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA
#  include <va/va.h>
#ਨਹੀਂਤਾਂ  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#ਜੇ_ਅੰਤ // HAVE_VA

ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#ਜੇ_ਅੰਤ // HAVE_OPENCL

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA_INTEL
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA_INTEL_OLD_ਸਿਰਲੇਖ
#  include <CL/va_ext.h>
#ਨਹੀਂਤਾਂ
#  include <CL/cl_va_api_media_sharing_intel.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OPENCV_LIBVA_LINK
#ਅਨ੍ਤਰ੍ਭਾਵ "va_wrapper.impl.hpp"
#ਨਹੀਂਤਾਂ
ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ detail {
ਸਥਿਰ ਖਾਲੀ init_libva() { /* nothing */ }
}}  // namespace
#ਜੇ_ਅੰਤ
ਵਰਤੋਂ ਨਾਮਾਕਾਸ਼ cv::detail;
#ਜੇ_ਅੰਤ

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ va_intel {

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA_INTEL

ਵਰਗ VAAPIInterop : ਜਨਤਕ ocl::Context::UserContext
{
ਜਨਤਕ:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_VA_APIMedia_Surfaces_INTEL  = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_VA_APIMedia_Surfaces_INTEL  = (cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_ਪਤਾ_For_Platform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        ਜੇ (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_VA_APIMedia_Surfaces_INTEL ||
            !cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_VA_APIMedia_Surfaces_INTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    ਆਭਾਸੀ ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_VA_APIMedia_Surfaces_INTEL_fn  cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_VA_APIMedia_Surfaces_INTEL;
    cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_VA_APIMedia_Surfaces_INTEL_fn  cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_VA_APIMedia_Surfaces_INTEL;
};

#ਜੇ_ਅੰਤ // HAVE_VA_INTEL

ਨਾਮਾਕਾਸ਼ ocl {

Context& initializeContextFromVA(VADisplay display, ਤਾਰਕਿਕ tryInterop)
{
    CV_ਅਣਵਰਤਿਆ(display); CV_ਅਣਵਰਤਿਆ(tryInterop);
#ਜੇ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    ਜੇ (tryInterop)
    {
        cl_uint ਅੰਕ_Platforms;
        cl_int ਸਥਿਤੀ = clGetPlatformIDs(0, ਨਿਰਾ, &ਅੰਕ_Platforms);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        ਜੇ (ਅੰਕ_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(ਅੰਕ_Platforms);
        ਸਥਿਤੀ = clGetPlatformIDs(ਅੰਕ_Platforms, &platforms[0], ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        ਪੂਰਨ ਮਿਲਿਆ = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        ਲਈ (ਪੂਰਨ i = 0; i < (ਪੂਰਨ)ਅੰਕ_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                cl_Get_Extension_Function_ਪਤਾ_For_Platform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            ਜੇ ((ਖਾਲੀ*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ਨਿਰਾ)
            {
                ਜਾਰੀ;
            }

            // Query device list

            cl_uint ਅੰਕ_Devices = 0;

            ਸਥਿਤੀ = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ਨਿਰਾ, &ਅੰਕ_Devices);
            ਜੇ ((ਸਥਿਤੀ != CL_SUCCESS) || !(ਅੰਕ_Devices > 0))
                ਜਾਰੀ;
            ਅੰਕ_Devices = 1; // OpenCV expects only 1 device
            ਸਥਿਤੀ = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, ਅੰਕ_Devices, &device, ਨਿਰਾ);
            ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
                ਜਾਰੀ;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, ਅੰਕ_Devices, &device, ਨਿਰਾ, ਨਿਰਾ, &ਸਥਿਤੀ);
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

        ਜੇ (ਮਿਲਿਆ >= 0)
        {
            cl_platform_id platform = platforms[ਮਿਲਿਆ];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            ਕੋਸ਼ਿਸ਼
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ਸੈੱਟ_User_Context(std::make_shared<VAAPIInterop>(platform));
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
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        ਵਾਪਸ ctx;
    }
#ਜੇ_ਅੰਤ  // !HAVE_VA
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA_INTEL
ਸਥਿਰ ਤਾਰਕਿਕ ocl_convert_nv12_ਮੰਜ਼ਿਲ_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_ਬਫਰ, ਪੂਰਨ ਕਦਮ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ ਪੰਕਤੀਆਂ)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(clImageY, clImageUV, cl_ਬਫਰ, ਕਦਮ, ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ);

    size_t globalsize[] = { (size_t)ਖਾਨੇ_ਗਿਣਤੀ/2, (size_t)ਪੰਕਤੀਆਂ/2 };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, 0, ਝੂਠ);
}

ਸਥਿਰ ਤਾਰਕਿਕ ocl_convert_bgr_ਮੰਜ਼ਿਲ_nv12(cl_mem cl_ਬਫਰ, ਪੂਰਨ ਕਦਮ, ਪੂਰਨ ਖਾਨੇ_ਗਿਣਤੀ, ਪੂਰਨ ਪੰਕਤੀਆਂ, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ਜੇ (k.empty())
        ਵਾਪਸ ਝੂਠ;

    k.ਦਲੀਲਾਂ(cl_ਬਫਰ, ਕਦਮ, ਖਾਨੇ_ਗਿਣਤੀ, ਪੰਕਤੀਆਂ, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)ਖਾਨੇ_ਗਿਣਤੀ/2, (size_t)ਪੰਕਤੀਆਂ/2 };
    ਵਾਪਸ k.ਲੜੀਵਾਰ(2, globalsize, 0, ਝੂਠ);
}
#ਜੇ_ਅੰਤ // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#ਜੇ defined(HAVE_VA)
ਅਚਲ ਪੂਰਨ NCHANNELS = 3;

ਸਥਿਰ ਖਾਲੀ copy_convert_nv12_ਮੰਜ਼ਿਲ_bgr(ਅਚਲ VAImage& image, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਬਫਰ, Mat& bgr)
{
    ਅਚਲ ਦਸ਼ਮਲਵ d1 = 16.0f;
    ਅਚਲ ਦਸ਼ਮਲਵ d2 = 128.0f;

    ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ਅਚਲ size_t src_ਵਿੱਥ_Y = image.offsets[0];
    ਅਚਲ size_t src_ਵਿੱਥ_UV = image.offsets[1];

    ਅਚਲ size_t src_ਕਦਮ_Y = image.pitches[0];
    ਅਚਲ size_t src_ਕਦਮ_UV = image.pitches[1];

    ਅਚਲ size_t dst_ਕਦਮ_3 = bgr.ਕਦਮ;

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcY0 = ਬਫਰ + src_ਵਿੱਥ_Y;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcUV = ਬਫਰ + src_ਵਿੱਥ_UV;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dst0 = bgr.ਅੰਕੜੇ;

    ਲਈ (ਪੂਰਨ y = 0; y < bgr.ਪੰਕਤੀਆਂ; y += 2)
    {
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcY1 = srcY0 + src_ਕਦਮ_Y;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *dst1 = dst0 + dst_ਕਦਮ_3;

        ਲਈ (ਪੂਰਨ x = 0; x < bgr.ਖਾਨੇ_ਗਿਣਤੀ; x += 2)
        {
            ਦਸ਼ਮਲਵ Y0 = ਦਸ਼ਮਲਵ(srcY0[x+0]);
            ਦਸ਼ਮਲਵ Y1 = ਦਸ਼ਮਲਵ(srcY0[x+1]);
            ਦਸ਼ਮਲਵ Y2 = ਦਸ਼ਮਲਵ(srcY1[x+0]);
            ਦਸ਼ਮਲਵ Y3 = ਦਸ਼ਮਲਵ(srcY1[x+1]);

            ਦਸ਼ਮਲਵ U = ਦਸ਼ਮਲਵ(srcUV[2*(x/2)+0]) - d2;
            ਦਸ਼ਮਲਵ V = ਦਸ਼ਮਲਵ(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ਦਸ਼ਮਲਵ ruv = coeffs[4]*V;
            ਦਸ਼ਮਲਵ guv = coeffs[3]*V + coeffs[2]*U;
            ਦਸ਼ਮਲਵ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3 + ruv);
        }

        srcY0 = srcY1 + src_ਕਦਮ_Y;
        srcUV += src_ਕਦਮ_UV;
        dst0 = dst1 + dst_ਕਦਮ_3;
    }
}

ਸਥਿਰ ਖਾਲੀ copy_convert_bgr_ਮੰਜ਼ਿਲ_nv12(ਅਚਲ VAImage& image, ਅਚਲ Mat& bgr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਬਫਰ)
{
    ਅਚਲ ਦਸ਼ਮਲਵ d1 = 16.0f;
    ਅਚਲ ਦਸ਼ਮਲਵ d2 = 128.0f;

    ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ਅਚਲ size_t dst_ਵਿੱਥ_Y = image.offsets[0];
    ਅਚਲ size_t dst_ਵਿੱਥ_UV = image.offsets[1];

    ਅਚਲ size_t dst_ਕਦਮ_Y = image.pitches[0];
    ਅਚਲ size_t dst_ਕਦਮ_UV = image.pitches[1];

    ਅਚਲ size_t src_ਕਦਮ_3 = bgr.ਕਦਮ;

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* src0 = bgr.ਅੰਕੜੇ;

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstY0 = ਬਫਰ + dst_ਵਿੱਥ_Y;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstUV = ਬਫਰ + dst_ਵਿੱਥ_UV;

    ਲਈ (ਪੂਰਨ y = 0; y < bgr.ਪੰਕਤੀਆਂ; y += 2)
    {
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *src1 = src0 + src_ਕਦਮ_3;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstY1 = dstY0 + dst_ਕਦਮ_Y;

        ਲਈ (ਪੂਰਨ x = 0; x < bgr.ਖਾਨੇ_ਗਿਣਤੀ; x += 2)
        {
            ਦਸ਼ਮਲਵ B0 = ਦਸ਼ਮਲਵ(src0[(x+0)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G0 = ਦਸ਼ਮਲਵ(src0[(x+0)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R0 = ਦਸ਼ਮਲਵ(src0[(x+0)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ B1 = ਦਸ਼ਮਲਵ(src0[(x+1)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G1 = ਦਸ਼ਮਲਵ(src0[(x+1)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R1 = ਦਸ਼ਮਲਵ(src0[(x+1)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ B2 = ਦਸ਼ਮਲਵ(src1[(x+0)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G2 = ਦਸ਼ਮਲਵ(src1[(x+0)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R2 = ਦਸ਼ਮਲਵ(src1[(x+0)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ B3 = ਦਸ਼ਮਲਵ(src1[(x+1)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G3 = ਦਸ਼ਮਲਵ(src1[(x+1)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R3 = ਦਸ਼ਮਲਵ(src1[(x+1)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ਦਸ਼ਮਲਵ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ਦਸ਼ਮਲਵ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ਦਸ਼ਮਲਵ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ਦਸ਼ਮਲਵ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ਦਸ਼ਮਲਵ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0);
            dstY0[x+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1);
            dstY1[x+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2);
            dstY1[x+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(U);
            dstUV[2*(x/2)+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(V);
        }

        src0 = src1 + src_ਕਦਮ_3;
        dstY0 = dstY1 + dst_ਕਦਮ_Y;
        dstUV += dst_ਕਦਮ_UV;
    }
}


ਸਥਿਰ ਖਾਲੀ copy_convert_yv12_ਮੰਜ਼ਿਲ_bgr(ਅਚਲ VAImage& image, ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਬਫਰ, Mat& bgr)
{
    ਅਚਲ ਦਸ਼ਮਲਵ d1 = 16.0f;
    ਅਚਲ ਦਸ਼ਮਲਵ d2 = 128.0f;

    ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ਅੰਕ_planes, (size_t)3, "");

    ਅਚਲ size_t src_ਵਿੱਥ_Y = image.offsets[0];
    ਅਚਲ size_t src_ਵਿੱਥ_V = image.offsets[1];
    ਅਚਲ size_t src_ਵਿੱਥ_U = image.offsets[2];

    ਅਚਲ size_t src_ਕਦਮ_Y = image.pitches[0];
    ਅਚਲ size_t src_ਕਦਮ_U = image.pitches[1];
    ਅਚਲ size_t src_ਕਦਮ_V = image.pitches[2];

    ਅਚਲ size_t dst_ਕਦਮ_3 = bgr.ਕਦਮ;

    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcY_ = ਬਫਰ + src_ਵਿੱਥ_Y;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcV_ = ਬਫਰ + src_ਵਿੱਥ_V;
    ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcU_ = ਬਫਰ + src_ਵਿੱਥ_U;

    ਲਈ (ਪੂਰਨ y = 0; y < bgr.ਪੰਕਤੀਆਂ; y += 2)
    {
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcY0 = srcY_ + (src_ਕਦਮ_Y) * y;
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcY1 = srcY0 + src_ਕਦਮ_Y;

        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcV = srcV_ + (src_ਕਦਮ_V) * y / 2;
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* srcU = srcU_ + (src_ਕਦਮ_U) * y / 2;

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dst0 = bgr.ਅੰਕੜੇ + (dst_ਕਦਮ_3) * y;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dst1 = dst0 + dst_ਕਦਮ_3;

        ਲਈ (ਪੂਰਨ x = 0; x < bgr.ਖਾਨੇ_ਗਿਣਤੀ; x += 2)
        {
            ਦਸ਼ਮਲਵ Y0 = ਦਸ਼ਮਲਵ(srcY0[x+0]);
            ਦਸ਼ਮਲਵ Y1 = ਦਸ਼ਮਲਵ(srcY0[x+1]);
            ਦਸ਼ਮਲਵ Y2 = ਦਸ਼ਮਲਵ(srcY1[x+0]);
            ਦਸ਼ਮਲਵ Y3 = ਦਸ਼ਮਲਵ(srcY1[x+1]);

            ਦਸ਼ਮਲਵ U = ਦਸ਼ਮਲਵ(srcU[x/2]) - d2;
            ਦਸ਼ਮਲਵ V = ਦਸ਼ਮਲਵ(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ਦਸ਼ਮਲਵ ruv = coeffs[4]*V;
            ਦਸ਼ਮਲਵ guv = coeffs[3]*V + coeffs[2]*U;
            ਦਸ਼ਮਲਵ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3 + ruv);
        }
    }
}

ਸਥਿਰ ਖਾਲੀ copy_convert_bgr_ਮੰਜ਼ਿਲ_yv12(ਅਚਲ VAImage& image, ਅਚਲ Mat& bgr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਬਫਰ)
{
    ਅਚਲ ਦਸ਼ਮਲਵ d1 = 16.0f;
    ਅਚਲ ਦਸ਼ਮਲਵ d2 = 128.0f;

    ਸਥਿਰ ਅਚਲ ਦਸ਼ਮਲਵ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ਅੰਕ_planes, (size_t)3, "");

    ਅਚਲ size_t dst_ਵਿੱਥ_Y = image.offsets[0];
    ਅਚਲ size_t dst_ਵਿੱਥ_V = image.offsets[1];
    ਅਚਲ size_t dst_ਵਿੱਥ_U = image.offsets[2];

    ਅਚਲ size_t dst_ਕਦਮ_Y = image.pitches[0];
    ਅਚਲ size_t dst_ਕਦਮ_U = image.pitches[1];
    ਅਚਲ size_t dst_ਕਦਮ_V = image.pitches[2];

    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstY_ = ਬਫਰ + dst_ਵਿੱਥ_Y;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstV_ = ਬਫਰ + dst_ਵਿੱਥ_V;
    ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstU_ = ਬਫਰ + dst_ਵਿੱਥ_U;

    ਅਚਲ size_t src_ਕਦਮ_3 = bgr.ਕਦਮ;

    ਲਈ (ਪੂਰਨ y = 0; y < bgr.ਪੰਕਤੀਆਂ; y += 2)
    {
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstY0 = dstY_ + (dst_ਕਦਮ_Y) * y;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstY1 = dstY0 + dst_ਕਦਮ_Y;

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstV = dstV_ + (dst_ਕਦਮ_V) * y / 2;
        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* dstU = dstU_ + (dst_ਕਦਮ_U) * y / 2;

        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* src0 = bgr.ਅੰਕੜੇ + (src_ਕਦਮ_3) * y;
        ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* src1 = src0 + src_ਕਦਮ_3;

        ਲਈ (ਪੂਰਨ x = 0; x < bgr.ਖਾਨੇ_ਗਿਣਤੀ; x += 2)
        {
            ਦਸ਼ਮਲਵ B0 = ਦਸ਼ਮਲਵ(src0[(x+0)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G0 = ਦਸ਼ਮਲਵ(src0[(x+0)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R0 = ਦਸ਼ਮਲਵ(src0[(x+0)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ B1 = ਦਸ਼ਮਲਵ(src0[(x+1)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G1 = ਦਸ਼ਮਲਵ(src0[(x+1)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R1 = ਦਸ਼ਮਲਵ(src0[(x+1)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ B2 = ਦਸ਼ਮਲਵ(src1[(x+0)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G2 = ਦਸ਼ਮਲਵ(src1[(x+0)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R2 = ਦਸ਼ਮਲਵ(src1[(x+0)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ B3 = ਦਸ਼ਮਲਵ(src1[(x+1)*NCHANNELS+0]);
            ਦਸ਼ਮਲਵ G3 = ਦਸ਼ਮਲਵ(src1[(x+1)*NCHANNELS+1]);
            ਦਸ਼ਮਲਵ R3 = ਦਸ਼ਮਲਵ(src1[(x+1)*NCHANNELS+2]);

            ਦਸ਼ਮਲਵ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ਦਸ਼ਮਲਵ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ਦਸ਼ਮਲਵ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ਦਸ਼ਮਲਵ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ਦਸ਼ਮਲਵ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ਦਸ਼ਮਲਵ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y0);
            dstY0[x+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y1);
            dstY1[x+0] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y2);
            dstY1[x+1] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(Y3);

            dstU[x/2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(U);
            dstV[x/2] = saturate_cast<ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ>(V);
        }
    }
}
#ਜੇ_ਅੰਤ // HAVE_VA

ਖਾਲੀ convert_ਮੰਜ਼ਿਲ_VASurface(VADisplay display, ਨਿਵੇਸ਼_Array src, VASurfaceID surface, ਮਾਪ size)
{
    CV_ਅਣਵਰਤਿਆ(display); CV_ਅਣਵਰਤਿਆ(src); CV_ਅਣਵਰਤਿਆ(surface); CV_ਅਣਵਰਤਿਆ(size);
#ਜੇ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ  // !HAVE_VA

    ਅਚਲ ਪੂਰਨ stype = CV_8UC3;

    ਪੂਰਨ srcType = src.type();
    CV_Assert(srcType == stype);

    ਮਾਪ src_ਮਾਪ = src.size();
    CV_Assert(src_ਮਾਪ.ਚੌੜਾਈ == size.ਚੌੜਾਈ && src_ਮਾਪ.ਉਚਾਈ == size.ਉਚਾਈ);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ਨਿਰਾ, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    ਖਾਲੀ* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ਨਿਰਾ, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    ਤਾਰਕਿਕ isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ਨਿਰਾ, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (ਖਾਲੀ*)display << "(surface)");
    ਜੇ (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int ਸਥਿਤੀ = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ocl_context.get_ਕਤਾਰ().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        ਸਥਿਤੀ = interop->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ਜੇ (!ocl::ocl_convert_bgr_ਮੰਜ਼ਿਲ_nv12(cl_ਬਫਰ, (ਪੂਰਨ)u.ਕਦਮ[0], u.ਖਾਨੇ_ਗਿਣਤੀ, u.ਪੰਕਤੀਆਂ, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        ਸਥਿਤੀ = clReleaseMemObject(clImageY); // TODO RAII
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        ਸਥਿਤੀ = clReleaseMemObject(clImageUV);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ਨਹੀਂਤਾਂ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.ਅੰਕੜੇ == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus ਸਥਿਤੀ = 0;

        ਸਥਿਤੀ = vaSyncSurface(display, surface);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        ਤਾਰਕਿਕ ਅਸਿੱਧਾ_ਬਫਰ = ਝੂਠ;
        VAImage image;
        ਸਥਿਤੀ = vaDeriveImage(display, surface, &image);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            ਅਸਿੱਧਾ_ਬਫਰ = ਸੱਚ;
            ਪੂਰਨ ਅੰਕ_formats = va_Max_ਅੰਕ_Image_Formats(display);
            ਜੇ (ਅੰਕ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ਅੰਕ_formats);

            ਸਥਿਤੀ = vaQueryImageFormats(display, fmt_list.ਅੰਕੜੇ(), &ਅੰਕ_formats);
            ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = ਖਾਲੀ_ਸੂਚਕ;
            ਲਈ (ਸਵੈਚਾਲਿਤ &fmt : fmt_list){
                ਜੇ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ਰੋਕੋ;
                }
            }
            ਜੇ (selected_format == ਖਾਲੀ_ਸੂਚਕ)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            ਸਥਿਤੀ = vaCreateImage(display, selected_format, size.ਚੌੜਾਈ, size.ਉਚਾਈ, &image);
            ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਬਫਰ = 0;
        ਸਥਿਤੀ = va_Map_ਬਫਰ(display, image.buf, (ਖਾਲੀ **)&ਬਫਰ);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ਜੇ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_ਮੰਜ਼ਿਲ_nv12(image, m, ਬਫਰ);
        ਜੇ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_ਮੰਜ਼ਿਲ_yv12(image, m, ਬਫਰ);
        ਨਹੀਂਤਾਂ
            CV_Check((ਪੂਰਨ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        ਸਥਿਤੀ = va_Unmap_ਬਫਰ(display, image.buf);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        ਜੇ (ਅਸਿੱਧਾ_ਬਫਰ){
            ਸਥਿਤੀ = vaPutImage(display, surface, image.image_id, 0, 0, size.ਚੌੜਾਈ, size.ਉਚਾਈ, 0, 0, size.ਚੌੜਾਈ, size.ਉਚਾਈ);
            ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        ਸਥਿਤੀ = vaDestroyImage(display, image.image_id);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#ਜੇ_ਅੰਤ  // !HAVE_VA
}

ਖਾਲੀ convertFromVASurface(VADisplay display, VASurfaceID surface, ਮਾਪ size, OutputArray dst)
{
    CV_ਅਣਵਰਤਿਆ(display); CV_ਅਣਵਰਤਿਆ(surface); CV_ਅਣਵਰਤਿਆ(dst); CV_ਅਣਵਰਤਿਆ(size);
#ਜੇ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ਨਹੀਂਤਾਂ  // !HAVE_VA

    ਅਚਲ ਪੂਰਨ dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    ਜੇ (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_ਬਫਰ = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int ਸਥਿਤੀ = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &ਸਥਿਤੀ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_ਕਤਾਰ q = (cl_command_ਕਤਾਰ)ocl_context.get_ਕਤਾਰ().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        ਸਥਿਤੀ = interop->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ਜੇ (!ocl::ocl_convert_nv12_ਮੰਜ਼ਿਲ_bgr(clImageY, clImageUV, cl_ਬਫਰ, (ਪੂਰਨ)u.ਕਦਮ[0], u.ਖਾਨੇ_ਗਿਣਤੀ, u.ਪੰਕਤੀਆਂ))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        ਸਥਿਤੀ = interop->cl_ਕਤਾਰ_ਵਿੱਚ_ਪਾਓ_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ਨਿਰਾ, ਨਿਰਾ);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        ਸਥਿਤੀ = cl_ਅੰਤ(q); // TODO Use events
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        ਸਥਿਤੀ = clReleaseMemObject(clImageY); // TODO RAII
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        ਸਥਿਤੀ = clReleaseMemObject(clImageUV);
        ਜੇ (ਸਥਿਤੀ != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ਨਹੀਂਤਾਂ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.ਅੰਕੜੇ == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus ਸਥਿਤੀ = 0;

        ਸਥਿਤੀ = vaSyncSurface(display, surface);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        ਸਥਿਤੀ = vaDeriveImage(display, surface, &image);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            ਪੂਰਨ ਅੰਕ_formats = va_Max_ਅੰਕ_Image_Formats(display);
            ਜੇ (ਅੰਕ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ਅੰਕ_formats);

            ਸਥਿਤੀ = vaQueryImageFormats(display, fmt_list.ਅੰਕੜੇ(), &ਅੰਕ_formats);
            ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = ਖਾਲੀ_ਸੂਚਕ;
            ਲਈ (ਸਵੈਚਾਲਿਤ &fmt : fmt_list){
                ਜੇ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ਰੋਕੋ;
                }
            }
            ਜੇ (selected_format == ਖਾਲੀ_ਸੂਚਕ)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            ਸਥਿਤੀ = vaCreateImage(display, selected_format, size.ਚੌੜਾਈ, size.ਉਚਾਈ, &image);
            ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            ਸਥਿਤੀ = vaGetImage(display, surface, 0, 0, size.ਚੌੜਾਈ, size.ਉਚਾਈ, image.image_id);
            ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ* ਬਫਰ = 0;
        ਸਥਿਤੀ = va_Map_ਬਫਰ(display, image.buf, (ਖਾਲੀ **)&ਬਫਰ);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ਜੇ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_ਮੰਜ਼ਿਲ_bgr(image, ਬਫਰ, m);
        ਜੇ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_ਮੰਜ਼ਿਲ_bgr(image, ਬਫਰ, m);
        ਨਹੀਂਤਾਂ
            CV_Check((ਪੂਰਨ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        ਸਥਿਤੀ = va_Unmap_ਬਫਰ(display, image.buf);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        ਸਥਿਤੀ = vaDestroyImage(display, image.image_id);
        ਜੇ (ਸਥਿਤੀ != VA_ਸਥਿਤੀ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#ਜੇ_ਅੰਤ  // !HAVE_VA
}

}} // namespace cv::va_intel
