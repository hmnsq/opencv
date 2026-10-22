// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA
#  include <va/va.h>
#ಇಲ್ಲದಿದ್ದರೆ  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#ಆದರೆ_ಅಂತ್ಯ // HAVE_VA

ಬಳಸಿ ನಾಮಾಕಾಶ cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#ಆದರೆ_ಅಂತ್ಯ // HAVE_OPENCL

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA_INTEL
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA_INTEL_OLD_ಶೀರ್ಷಿಕೆ
#  include <CL/va_ext.h>
#ಇಲ್ಲದಿದ್ದರೆ
#  include <CL/cl_va_api_media_sharing_intel.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OPENCV_LIBVA_LINK
#ಅನ್ತರ್ಭಾವ "va_wrapper.impl.hpp"
#ಇಲ್ಲದಿದ್ದರೆ
ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ detail {
ಸ್ಥಿರ ಶೂನ್ಯ init_libva() { /* nothing */ }
}}  // namespace
#ಆದರೆ_ಅಂತ್ಯ
ಬಳಸಿ ನಾಮಾಕಾಶ cv::detail;
#ಆದರೆ_ಅಂತ್ಯ

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ va_intel {

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA_INTEL

ವರ್ಗ VAAPIInterop : ಸಾರ್ವಜನಿಕ ocl::Context::UserContext
{
ಸಾರ್ವಜನಿಕ:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_VA_APIMedia_Surfaces_INTEL  = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_VA_APIMedia_Surfaces_INTEL  = (cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        ಆದರೆ (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_VA_APIMedia_Surfaces_INTEL ||
            !cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_VA_APIMedia_Surfaces_INTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    ವಾಸ್ತವಿಕ ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_VA_APIMedia_Surfaces_INTEL_fn  cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_VA_APIMedia_Surfaces_INTEL;
    cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_VA_APIMedia_Surfaces_INTEL_fn  cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_VA_APIMedia_Surfaces_INTEL;
};

#ಆದರೆ_ಅಂತ್ಯ // HAVE_VA_INTEL

ನಾಮಾಕಾಶ ocl {

Context& initializeContextFromVA(VADisplay display, ತಾರ್ಕಿಕ tryInterop)
{
    CV_ಬಳಸದ(display); CV_ಬಳಸದ(tryInterop);
#ಆದರೆ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    ಆದರೆ (tryInterop)
    {
        cl_uint ಅಂಕೆ_Platforms;
        cl_int ಸ್ಥಿತಿ_2 = clGetPlatformIDs(0, ಖಾಲಿ, &ಅಂಕೆ_Platforms);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        ಆದರೆ (ಅಂಕೆ_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(ಅಂಕೆ_Platforms);
        ಸ್ಥಿತಿ_2 = clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        ಪೂರ್ಣಾಂಕ ಸಿಕ್ಕಿತು = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < (ಪೂರ್ಣಾಂಕ)ಅಂಕೆ_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                cl_Get_Extension_Function_ವಿಳಾಸ_For_Platform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            ಆದರೆ ((ಶೂನ್ಯ*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ಖಾಲಿ)
            {
                ಮುಂದುವರಿ;
            }

            // Query device list

            cl_uint ಅಂಕೆ_Devices = 0;

            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ಖಾಲಿ, &ಅಂಕೆ_Devices);
            ಆದರೆ ((ಸ್ಥಿತಿ_2 != CL_SUCCESS) || !(ಅಂಕೆ_Devices > 0))
                ಮುಂದುವರಿ;
            ಅಂಕೆ_Devices = 1; // OpenCV expects only 1 device
            ಸ್ಥಿತಿ_2 = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, ಅಂಕೆ_Devices, &device, ಖಾಲಿ);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
                ಮುಂದುವರಿ;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, ಅಂಕೆ_Devices, &device, ಖಾಲಿ, ಖಾಲಿ, &ಸ್ಥಿತಿ_2);
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

        ಆದರೆ (ಸಿಕ್ಕಿತು >= 0)
        {
            cl_platform_id platform = platforms[ಸಿಕ್ಕಿತು];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            ಪ್ರಯತ್ನಿಸು
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().ಗಣ_User_Context(std::make_shared<VAAPIInterop>(platform));
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
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        ಹಿಂತಿರುಗಿ ctx;
    }
#ಆದರೆ_ಅಂತ್ಯ  // !HAVE_VA
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA_INTEL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convert_nv12_ಗಮ್ಯ_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_ಬಫರ್, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(clImageY, clImageUV, cl_ಬಫರ್, ಹೆಜ್ಜೆ, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು);

    size_t globalsize[] = { (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/2, (size_t)ಅಡ್ಡಸಾಲುಗಳು/2 };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, 0, ಸುಳ್ಳು);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convert_bgr_ಗಮ್ಯ_nv12(cl_mem cl_ಬಫರ್, ಪೂರ್ಣಾಂಕ ಹೆಜ್ಜೆ, ಪೂರ್ಣಾಂಕ ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಪೂರ್ಣಾಂಕ ಅಡ್ಡಸಾಲುಗಳು, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ಆದರೆ (k.empty())
        ಹಿಂತಿರುಗಿ ಸುಳ್ಳು;

    k.ವಾದಗಳು(cl_ಬಫರ್, ಹೆಜ್ಜೆ, ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, ಅಡ್ಡಸಾಲುಗಳು, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ/2, (size_t)ಅಡ್ಡಸಾಲುಗಳು/2 };
    ಹಿಂತಿರುಗಿ k.ಸತತ(2, globalsize, 0, ಸುಳ್ಳು);
}
#ಆದರೆ_ಅಂತ್ಯ // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#ಆದರೆ defined(HAVE_VA)
ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ NCHANNELS = 3;

ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_nv12_ಗಮ್ಯ_bgr(ಸ್ಥಿರಾಂಕ VAImage& image, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಬಫರ್, Mat& bgr)
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d1 = 16.0f;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d2 = 128.0f;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ಸ್ಥಿರಾಂಕ size_t src_ಸ್ಥಾನಾಂತರ_Y = image.offsets[0];
    ಸ್ಥಿರಾಂಕ size_t src_ಸ್ಥಾನಾಂತರ_UV = image.offsets[1];

    ಸ್ಥಿರಾಂಕ size_t src_ಹೆಜ್ಜೆ_Y = image.pitches[0];
    ಸ್ಥಿರಾಂಕ size_t src_ಹೆಜ್ಜೆ_UV = image.pitches[1];

    ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ_3 = bgr.ಹೆಜ್ಜೆ;

    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcY0 = ಬಫರ್ + src_ಸ್ಥಾನಾಂತರ_Y;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcUV = ಬಫರ್ + src_ಸ್ಥಾನಾಂತರ_UV;

    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dst0 = bgr.ದತ್ತಾಂಶ;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < bgr.ಅಡ್ಡಸಾಲುಗಳು; y += 2)
    {
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcY1 = srcY0 + src_ಹೆಜ್ಜೆ_Y;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *dst1 = dst0 + dst_ಹೆಜ್ಜೆ_3;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < bgr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x += 2)
        {
            ದಶಮಾಂಶ Y0 = ದಶಮಾಂಶ(srcY0[x+0]);
            ದಶಮಾಂಶ Y1 = ದಶಮಾಂಶ(srcY0[x+1]);
            ದಶಮಾಂಶ Y2 = ದಶಮಾಂಶ(srcY1[x+0]);
            ದಶಮಾಂಶ Y3 = ದಶಮಾಂಶ(srcY1[x+1]);

            ದಶಮಾಂಶ U = ದಶಮಾಂಶ(srcUV[2*(x/2)+0]) - d2;
            ದಶಮಾಂಶ V = ದಶಮಾಂಶ(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ದಶಮಾಂಶ ruv = coeffs[4]*V;
            ದಶಮಾಂಶ guv = coeffs[3]*V + coeffs[2]*U;
            ದಶಮಾಂಶ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3 + ruv);
        }

        srcY0 = srcY1 + src_ಹೆಜ್ಜೆ_Y;
        srcUV += src_ಹೆಜ್ಜೆ_UV;
        dst0 = dst1 + dst_ಹೆಜ್ಜೆ_3;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_bgr_ಗಮ್ಯ_nv12(ಸ್ಥಿರಾಂಕ VAImage& image, ಸ್ಥಿರಾಂಕ Mat& bgr, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಬಫರ್)
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d1 = 16.0f;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d2 = 128.0f;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ಸ್ಥಿರಾಂಕ size_t dst_ಸ್ಥಾನಾಂತರ_Y = image.offsets[0];
    ಸ್ಥಿರಾಂಕ size_t dst_ಸ್ಥಾನಾಂತರ_UV = image.offsets[1];

    ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ_Y = image.pitches[0];
    ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ_UV = image.pitches[1];

    ಸ್ಥಿರಾಂಕ size_t src_ಹೆಜ್ಜೆ_3 = bgr.ಹೆಜ್ಜೆ;

    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* src0 = bgr.ದತ್ತಾಂಶ;

    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstY0 = ಬಫರ್ + dst_ಸ್ಥಾನಾಂತರ_Y;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstUV = ಬಫರ್ + dst_ಸ್ಥಾನಾಂತರ_UV;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < bgr.ಅಡ್ಡಸಾಲುಗಳು; y += 2)
    {
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *src1 = src0 + src_ಹೆಜ್ಜೆ_3;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstY1 = dstY0 + dst_ಹೆಜ್ಜೆ_Y;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < bgr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x += 2)
        {
            ದಶಮಾಂಶ B0 = ದಶಮಾಂಶ(src0[(x+0)*NCHANNELS+0]);
            ದಶಮಾಂಶ G0 = ದಶಮಾಂಶ(src0[(x+0)*NCHANNELS+1]);
            ದಶಮಾಂಶ R0 = ದಶಮಾಂಶ(src0[(x+0)*NCHANNELS+2]);

            ದಶಮಾಂಶ B1 = ದಶಮಾಂಶ(src0[(x+1)*NCHANNELS+0]);
            ದಶಮಾಂಶ G1 = ದಶಮಾಂಶ(src0[(x+1)*NCHANNELS+1]);
            ದಶಮಾಂಶ R1 = ದಶಮಾಂಶ(src0[(x+1)*NCHANNELS+2]);

            ದಶಮಾಂಶ B2 = ದಶಮಾಂಶ(src1[(x+0)*NCHANNELS+0]);
            ದಶಮಾಂಶ G2 = ದಶಮಾಂಶ(src1[(x+0)*NCHANNELS+1]);
            ದಶಮಾಂಶ R2 = ದಶಮಾಂಶ(src1[(x+0)*NCHANNELS+2]);

            ದಶಮಾಂಶ B3 = ದಶಮಾಂಶ(src1[(x+1)*NCHANNELS+0]);
            ದಶಮಾಂಶ G3 = ದಶಮಾಂಶ(src1[(x+1)*NCHANNELS+1]);
            ದಶಮಾಂಶ R3 = ದಶಮಾಂಶ(src1[(x+1)*NCHANNELS+2]);

            ದಶಮಾಂಶ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ದಶಮಾಂಶ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ದಶಮಾಂಶ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ದಶಮಾಂಶ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ದಶಮಾಂಶ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ದಶಮಾಂಶ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0);
            dstY0[x+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1);
            dstY1[x+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2);
            dstY1[x+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(U);
            dstUV[2*(x/2)+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(V);
        }

        src0 = src1 + src_ಹೆಜ್ಜೆ_3;
        dstY0 = dstY1 + dst_ಹೆಜ್ಜೆ_Y;
        dstUV += dst_ಹೆಜ್ಜೆ_UV;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_yv12_ಗಮ್ಯ_bgr(ಸ್ಥಿರಾಂಕ VAImage& image, ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಬಫರ್, Mat& bgr)
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d1 = 16.0f;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d2 = 128.0f;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ಅಂಕೆ_planes, (size_t)3, "");

    ಸ್ಥಿರಾಂಕ size_t src_ಸ್ಥಾನಾಂತರ_Y = image.offsets[0];
    ಸ್ಥಿರಾಂಕ size_t src_ಸ್ಥಾನಾಂತರ_V = image.offsets[1];
    ಸ್ಥಿರಾಂಕ size_t src_ಸ್ಥಾನಾಂತರ_U = image.offsets[2];

    ಸ್ಥಿರಾಂಕ size_t src_ಹೆಜ್ಜೆ_Y = image.pitches[0];
    ಸ್ಥಿರಾಂಕ size_t src_ಹೆಜ್ಜೆ_U = image.pitches[1];
    ಸ್ಥಿರಾಂಕ size_t src_ಹೆಜ್ಜೆ_V = image.pitches[2];

    ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ_3 = bgr.ಹೆಜ್ಜೆ;

    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcY_ = ಬಫರ್ + src_ಸ್ಥಾನಾಂತರ_Y;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcV_ = ಬಫರ್ + src_ಸ್ಥಾನಾಂತರ_V;
    ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcU_ = ಬಫರ್ + src_ಸ್ಥಾನಾಂತರ_U;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < bgr.ಅಡ್ಡಸಾಲುಗಳು; y += 2)
    {
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcY0 = srcY_ + (src_ಹೆಜ್ಜೆ_Y) * y;
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcY1 = srcY0 + src_ಹೆಜ್ಜೆ_Y;

        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcV = srcV_ + (src_ಹೆಜ್ಜೆ_V) * y / 2;
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* srcU = srcU_ + (src_ಹೆಜ್ಜೆ_U) * y / 2;

        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dst0 = bgr.ದತ್ತಾಂಶ + (dst_ಹೆಜ್ಜೆ_3) * y;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dst1 = dst0 + dst_ಹೆಜ್ಜೆ_3;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < bgr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x += 2)
        {
            ದಶಮಾಂಶ Y0 = ದಶಮಾಂಶ(srcY0[x+0]);
            ದಶಮಾಂಶ Y1 = ದಶಮಾಂಶ(srcY0[x+1]);
            ದಶಮಾಂಶ Y2 = ದಶಮಾಂಶ(srcY1[x+0]);
            ದಶಮಾಂಶ Y3 = ದಶಮಾಂಶ(srcY1[x+1]);

            ದಶಮಾಂಶ U = ದಶಮಾಂಶ(srcU[x/2]) - d2;
            ದಶಮಾಂಶ V = ದಶಮಾಂಶ(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ದಶಮಾಂಶ ruv = coeffs[4]*V;
            ದಶಮಾಂಶ guv = coeffs[3]*V + coeffs[2]*U;
            ದಶಮಾಂಶ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3 + ruv);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_bgr_ಗಮ್ಯ_yv12(ಸ್ಥಿರಾಂಕ VAImage& image, ಸ್ಥಿರಾಂಕ Mat& bgr, ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಬಫರ್)
{
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d1 = 16.0f;
    ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ d2 = 128.0f;

    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ದಶಮಾಂಶ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ಅಂಕೆ_planes, (size_t)3, "");

    ಸ್ಥಿರಾಂಕ size_t dst_ಸ್ಥಾನಾಂತರ_Y = image.offsets[0];
    ಸ್ಥಿರಾಂಕ size_t dst_ಸ್ಥಾನಾಂತರ_V = image.offsets[1];
    ಸ್ಥಿರಾಂಕ size_t dst_ಸ್ಥಾನಾಂತರ_U = image.offsets[2];

    ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ_Y = image.pitches[0];
    ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ_U = image.pitches[1];
    ಸ್ಥಿರಾಂಕ size_t dst_ಹೆಜ್ಜೆ_V = image.pitches[2];

    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstY_ = ಬಫರ್ + dst_ಸ್ಥಾನಾಂತರ_Y;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstV_ = ಬಫರ್ + dst_ಸ್ಥಾನಾಂತರ_V;
    ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstU_ = ಬಫರ್ + dst_ಸ್ಥಾನಾಂತರ_U;

    ಸ್ಥಿರಾಂಕ size_t src_ಹೆಜ್ಜೆ_3 = bgr.ಹೆಜ್ಜೆ;

    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ y = 0; y < bgr.ಅಡ್ಡಸಾಲುಗಳು; y += 2)
    {
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstY0 = dstY_ + (dst_ಹೆಜ್ಜೆ_Y) * y;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstY1 = dstY0 + dst_ಹೆಜ್ಜೆ_Y;

        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstV = dstV_ + (dst_ಹೆಜ್ಜೆ_V) * y / 2;
        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* dstU = dstU_ + (dst_ಹೆಜ್ಜೆ_U) * y / 2;

        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* src0 = bgr.ದತ್ತಾಂಶ + (src_ಹೆಜ್ಜೆ_3) * y;
        ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* src1 = src0 + src_ಹೆಜ್ಜೆ_3;

        ಗಾಗಿ (ಪೂರ್ಣಾಂಕ x = 0; x < bgr.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ; x += 2)
        {
            ದಶಮಾಂಶ B0 = ದಶಮಾಂಶ(src0[(x+0)*NCHANNELS+0]);
            ದಶಮಾಂಶ G0 = ದಶಮಾಂಶ(src0[(x+0)*NCHANNELS+1]);
            ದಶಮಾಂಶ R0 = ದಶಮಾಂಶ(src0[(x+0)*NCHANNELS+2]);

            ದಶಮಾಂಶ B1 = ದಶಮಾಂಶ(src0[(x+1)*NCHANNELS+0]);
            ದಶಮಾಂಶ G1 = ದಶಮಾಂಶ(src0[(x+1)*NCHANNELS+1]);
            ದಶಮಾಂಶ R1 = ದಶಮಾಂಶ(src0[(x+1)*NCHANNELS+2]);

            ದಶಮಾಂಶ B2 = ದಶಮಾಂಶ(src1[(x+0)*NCHANNELS+0]);
            ದಶಮಾಂಶ G2 = ದಶಮಾಂಶ(src1[(x+0)*NCHANNELS+1]);
            ದಶಮಾಂಶ R2 = ದಶಮಾಂಶ(src1[(x+0)*NCHANNELS+2]);

            ದಶಮಾಂಶ B3 = ದಶಮಾಂಶ(src1[(x+1)*NCHANNELS+0]);
            ದಶಮಾಂಶ G3 = ದಶಮಾಂಶ(src1[(x+1)*NCHANNELS+1]);
            ದಶಮಾಂಶ R3 = ದಶಮಾಂಶ(src1[(x+1)*NCHANNELS+2]);

            ದಶಮಾಂಶ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ದಶಮಾಂಶ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ದಶಮಾಂಶ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ದಶಮಾಂಶ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ದಶಮಾಂಶ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ದಶಮಾಂಶ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y0);
            dstY0[x+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y1);
            dstY1[x+0] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y2);
            dstY1[x+1] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(Y3);

            dstU[x/2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(U);
            dstV[x/2] = saturate_cast<ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ>(V);
        }
    }
}
#ಆದರೆ_ಅಂತ್ಯ // HAVE_VA

ಶೂನ್ಯ convert_ಗಮ್ಯ_VASurface(VADisplay display, ಒಳಹರಿವು_Array src, VASurfaceID surface, ಗಾತ್ರ_2 size)
{
    CV_ಬಳಸದ(display); CV_ಬಳಸದ(src); CV_ಬಳಸದ(surface); CV_ಬಳಸದ(size);
#ಆದರೆ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ  // !HAVE_VA

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ stype = CV_8UC3;

    ಪೂರ್ಣಾಂಕ srcType = src.type();
    CV_Assert(srcType == stype);

    ಗಾತ್ರ_2 src_ಗಾತ್ರ = src.size();
    CV_Assert(src_ಗಾತ್ರ.ಅಗಲ == size.ಅಗಲ && src_ಗಾತ್ರ.ಎತ್ತರ == size.ಎತ್ತರ);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ಖಾಲಿ, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    ಶೂನ್ಯ* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ಖಾಲಿ, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    ತಾರ್ಕಿಕ isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ಖಾಲಿ, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (ಶೂನ್ಯ*)display << "(surface)");
    ಆದರೆ (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int ಸ್ಥಿತಿ_2 = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ocl_context.get_ಸರತಿ().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        ಸ್ಥಿತಿ_2 = interop->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ಆದರೆ (!ocl::ocl_convert_bgr_ಗಮ್ಯ_nv12(cl_ಬಫರ್, (ಪೂರ್ಣಾಂಕ)u.ಹೆಜ್ಜೆ[0], u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, u.ಅಡ್ಡಸಾಲುಗಳು, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageY); // TODO RAII
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageUV);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ಇಲ್ಲದಿದ್ದರೆ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.ದತ್ತಾಂಶ == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus ಸ್ಥಿತಿ_2 = 0;

        ಸ್ಥಿತಿ_2 = vaSyncSurface(display, surface);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        ತಾರ್ಕಿಕ ಪರೋಕ್ಷ_ಬಫರ್ = ಸುಳ್ಳು;
        VAImage image;
        ಸ್ಥಿತಿ_2 = vaDeriveImage(display, surface, &image);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            ಪರೋಕ್ಷ_ಬಫರ್ = ನಿಜ;
            ಪೂರ್ಣಾಂಕ ಅಂಕೆ_formats = va_Max_ಅಂಕೆ_Image_Formats(display);
            ಆದರೆ (ಅಂಕೆ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ಅಂಕೆ_formats);

            ಸ್ಥಿತಿ_2 = vaQueryImageFormats(display, fmt_list.ದತ್ತಾಂಶ(), &ಅಂಕೆ_formats);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = ಶೂನ್ಯಸೂಚಕ;
            ಗಾಗಿ (ಸ್ವಯಂಚಾಲಿತ &fmt : fmt_list){
                ಆದರೆ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ನಿಲ್ಲಿಸು;
                }
            }
            ಆದರೆ (selected_format == ಶೂನ್ಯಸೂಚಕ)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            ಸ್ಥಿತಿ_2 = vaCreateImage(display, selected_format, size.ಅಗಲ, size.ಎತ್ತರ, &image);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಬಫರ್ = 0;
        ಸ್ಥಿತಿ_2 = va_Map_ಬಫರ್(display, image.buf, (ಶೂನ್ಯ **)&ಬಫರ್);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ಆದರೆ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_ಗಮ್ಯ_nv12(image, m, ಬಫರ್);
        ಆದರೆ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_ಗಮ್ಯ_yv12(image, m, ಬಫರ್);
        ಇಲ್ಲದಿದ್ದರೆ
            CV_Check((ಪೂರ್ಣಾಂಕ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        ಸ್ಥಿತಿ_2 = va_Unmap_ಬಫರ್(display, image.buf);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        ಆದರೆ (ಪರೋಕ್ಷ_ಬಫರ್){
            ಸ್ಥಿತಿ_2 = vaPutImage(display, surface, image.image_id, 0, 0, size.ಅಗಲ, size.ಎತ್ತರ, 0, 0, size.ಅಗಲ, size.ಎತ್ತರ);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        ಸ್ಥಿತಿ_2 = vaDestroyImage(display, image.image_id);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#ಆದರೆ_ಅಂತ್ಯ  // !HAVE_VA
}

ಶೂನ್ಯ convertFromVASurface(VADisplay display, VASurfaceID surface, ಗಾತ್ರ_2 size, OutputArray dst)
{
    CV_ಬಳಸದ(display); CV_ಬಳಸದ(surface); CV_ಬಳಸದ(dst); CV_ಬಳಸದ(size);
#ಆದರೆ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ಇಲ್ಲದಿದ್ದರೆ  // !HAVE_VA

    ಸ್ಥಿರಾಂಕ ಪೂರ್ಣಾಂಕ dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    ಆದರೆ (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_ಬಫರ್ = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int ಸ್ಥಿತಿ_2 = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &ಸ್ಥಿತಿ_2);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_ಸರತಿ q = (cl_command_ಸರತಿ)ocl_context.get_ಸರತಿ().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        ಸ್ಥಿತಿ_2 = interop->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ಆದರೆ (!ocl::ocl_convert_nv12_ಗಮ್ಯ_bgr(clImageY, clImageUV, cl_ಬಫರ್, (ಪೂರ್ಣಾಂಕ)u.ಹೆಜ್ಜೆ[0], u.ಲಂಬಸಾಲು_ಸಂಖ್ಯೆ, u.ಅಡ್ಡಸಾಲುಗಳು))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        ಸ್ಥಿತಿ_2 = interop->cl_ಸರತಿಗೆ_ಸೇರಿಸು_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ಖಾಲಿ, ಖಾಲಿ);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        ಸ್ಥಿತಿ_2 = cl_ಮುಕ್ತಾಯ(q); // TODO Use events
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageY); // TODO RAII
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        ಸ್ಥಿತಿ_2 = clReleaseMemObject(clImageUV);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ಇಲ್ಲದಿದ್ದರೆ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.ದತ್ತಾಂಶ == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus ಸ್ಥಿತಿ_2 = 0;

        ಸ್ಥಿತಿ_2 = vaSyncSurface(display, surface);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        ಸ್ಥಿತಿ_2 = vaDeriveImage(display, surface, &image);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            ಪೂರ್ಣಾಂಕ ಅಂಕೆ_formats = va_Max_ಅಂಕೆ_Image_Formats(display);
            ಆದರೆ (ಅಂಕೆ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ಅಂಕೆ_formats);

            ಸ್ಥಿತಿ_2 = vaQueryImageFormats(display, fmt_list.ದತ್ತಾಂಶ(), &ಅಂಕೆ_formats);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = ಶೂನ್ಯಸೂಚಕ;
            ಗಾಗಿ (ಸ್ವಯಂಚಾಲಿತ &fmt : fmt_list){
                ಆದರೆ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ನಿಲ್ಲಿಸು;
                }
            }
            ಆದರೆ (selected_format == ಶೂನ್ಯಸೂಚಕ)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            ಸ್ಥಿತಿ_2 = vaCreateImage(display, selected_format, size.ಅಗಲ, size.ಎತ್ತರ, &image);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            ಸ್ಥಿತಿ_2 = vaGetImage(display, surface, 0, 0, size.ಅಗಲ, size.ಎತ್ತರ, image.image_id);
            ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ* ಬಫರ್ = 0;
        ಸ್ಥಿತಿ_2 = va_Map_ಬಫರ್(display, image.buf, (ಶೂನ್ಯ **)&ಬಫರ್);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ಆದರೆ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_ಗಮ್ಯ_bgr(image, ಬಫರ್, m);
        ಆದರೆ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_ಗಮ್ಯ_bgr(image, ಬಫರ್, m);
        ಇಲ್ಲದಿದ್ದರೆ
            CV_Check((ಪೂರ್ಣಾಂಕ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        ಸ್ಥಿತಿ_2 = va_Unmap_ಬಫರ್(display, image.buf);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        ಸ್ಥಿತಿ_2 = vaDestroyImage(display, image.image_id);
        ಆದರೆ (ಸ್ಥಿತಿ_2 != VA_ಸ್ಥಿತಿ_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#ಆದರೆ_ಅಂತ್ಯ  // !HAVE_VA
}

}} // namespace cv::va_intel
