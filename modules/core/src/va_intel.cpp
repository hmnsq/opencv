// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ಅನ್ತರ್ಭಾವ "precomp.hpp"

#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#ಅನ್ಯಥಾ  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using ನಾಮಾಕಾಶ cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#ifdef HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#endif // HAVE_OPENCL

#ifdef HAVE_VA_INTEL
#ifdef HAVE_VA_INTEL_OLD_HEADER
#  include <CL/va_ext.h>
#ಅನ್ಯಥಾ
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#ಅನ್ತರ್ಭಾವ "va_wrapper.impl.hpp"
#ಅನ್ಯಥಾ
ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ detail {
ಸ್ಥಿರ ಶೂನ್ಯ init_libva() { /* nothing */ }
}}  // namespace
#endif
using ನಾಮಾಕಾಶ cv::detail;
#endif

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ va_intel {

#ifdef HAVE_VA_INTEL

ವರ್ಗ VAAPIInterop : ಸಾರ್ವಜನಿಕ ocl::Context::UserContext
{
ಸಾರ್ವಜನಿಕ:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        ಯದಿ (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    ಆಭಾಸ ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

ನಾಮಾಕಾಶ ocl {

Context& initializeContextFromVA(VADisplay display, ತಾರ್ಕಿಕ tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#ಯದಿ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ಅನ್ಯಥಾ  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    ಯದಿ (tryInterop)
    {
        cl_uint ಅಂಕೆ_Platforms;
        cl_int status = clGetPlatformIDs(0, ರಿಕ್ತ, &ಅಂಕೆ_Platforms);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        ಯದಿ (ಅಂಕೆ_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(ಅಂಕೆ_Platforms);
        status = clGetPlatformIDs(ಅಂಕೆ_Platforms, &platforms[0], ರಿಕ್ತ);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        ಪೂರ್ಣಾಙ್ಕ found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < (ಪೂರ್ಣಾಙ್ಕ)ಅಂಕೆ_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            ಯದಿ ((ಶೂನ್ಯ*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ರಿಕ್ತ)
            {
                ಅನುವರ್ತ;
            }

            // Query device list

            cl_uint ಅಂಕೆ_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ರಿಕ್ತ, &ಅಂಕೆ_Devices);
            ಯದಿ ((status != CL_SUCCESS) || !(ಅಂಕೆ_Devices > 0))
                ಅನುವರ್ತ;
            ಅಂಕೆ_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, ಅಂಕೆ_Devices, &device, ರಿಕ್ತ);
            ಯದಿ (status != CL_SUCCESS)
                ಅನುವರ್ತ;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, ಅಂಕೆ_Devices, &device, ರಿಕ್ತ, ರಿಕ್ತ, &status);
            ಯದಿ (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            ಅನ್ಯಥಾ
            {
                found = i;
                ಭಙ್ಗ;
            }
        }

        ಯದಿ (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            ಯತ್ನ
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            ಗೃಹಾಣ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ಕ್ಷಿಪ;
            }
            clExecCtx.bind();
            ಪ್ರತ್ಯಾಗಮ const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        ಪ್ರತ್ಯಾಗಮ ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, ಪೂರ್ಣಾಙ್ಕ step, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, 0, ಅಸತ್ಯ);
}

ಸ್ಥಿರ ತಾರ್ಕಿಕ ocl_convert_bgr_to_nv12(cl_mem clBuffer, ಪೂರ್ಣಾಙ್ಕ step, ಪೂರ್ಣಾಙ್ಕ cols, ಪೂರ್ಣಾಙ್ಕ rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ಯದಿ (k.empty())
        ಪ್ರತ್ಯಾಗಮ ಅಸತ್ಯ;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ಪ್ರತ್ಯಾಗಮ k.run(2, globalsize, 0, ಅಸತ್ಯ);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#ಯದಿ defined(HAVE_VA)
ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ NCHANNELS = 3;

ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_nv12_to_bgr(ಧ್ರುವ VAImage& image, ಧ್ರುವ unsigned ವರ್ಣ* buffer, Mat& bgr)
{
    ಧ್ರುವ ಭಿನ್ನ d1 = 16.0f;
    ಧ್ರುವ ಭಿನ್ನ d2 = 128.0f;

    ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ಧ್ರುವ size_t srcOffsetY = image.offsets[0];
    ಧ್ರುವ size_t srcOffsetUV = image.offsets[1];

    ಧ್ರುವ size_t srcStepY = image.pitches[0];
    ಧ್ರುವ size_t srcStepUV = image.pitches[1];

    ಧ್ರುವ size_t dstStep = bgr.step;

    ಧ್ರುವ unsigned ವರ್ಣ* srcY0 = buffer + srcOffsetY;
    ಧ್ರುವ unsigned ವರ್ಣ* srcUV = buffer + srcOffsetUV;

    unsigned ವರ್ಣ* dst0 = bgr.data;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < bgr.rows; y += 2)
    {
        ಧ್ರುವ unsigned ವರ್ಣ* srcY1 = srcY0 + srcStepY;
        unsigned ವರ್ಣ *dst1 = dst0 + dstStep;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < bgr.cols; x += 2)
        {
            ಭಿನ್ನ Y0 = ಭಿನ್ನ(srcY0[x+0]);
            ಭಿನ್ನ Y1 = ಭಿನ್ನ(srcY0[x+1]);
            ಭಿನ್ನ Y2 = ಭಿನ್ನ(srcY1[x+0]);
            ಭಿನ್ನ Y3 = ಭಿನ್ನ(srcY1[x+1]);

            ಭಿನ್ನ U = ಭಿನ್ನ(srcUV[2*(x/2)+0]) - d2;
            ಭಿನ್ನ V = ಭಿನ್ನ(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ಭಿನ್ನ ruv = coeffs[4]*V;
            ಭಿನ್ನ guv = coeffs[3]*V + coeffs[2]*U;
            ಭಿನ್ನ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_bgr_to_nv12(ಧ್ರುವ VAImage& image, ಧ್ರುವ Mat& bgr, unsigned ವರ್ಣ* buffer)
{
    ಧ್ರುವ ಭಿನ್ನ d1 = 16.0f;
    ಧ್ರುವ ಭಿನ್ನ d2 = 128.0f;

    ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ಧ್ರುವ size_t dstOffsetY = image.offsets[0];
    ಧ್ರುವ size_t dstOffsetUV = image.offsets[1];

    ಧ್ರುವ size_t dstStepY = image.pitches[0];
    ಧ್ರುವ size_t dstStepUV = image.pitches[1];

    ಧ್ರುವ size_t srcStep = bgr.step;

    ಧ್ರುವ unsigned ವರ್ಣ* src0 = bgr.data;

    unsigned ವರ್ಣ* dstY0 = buffer + dstOffsetY;
    unsigned ವರ್ಣ* dstUV = buffer + dstOffsetUV;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < bgr.rows; y += 2)
    {
        ಧ್ರುವ unsigned ವರ್ಣ *src1 = src0 + srcStep;
        unsigned ವರ್ಣ* dstY1 = dstY0 + dstStepY;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < bgr.cols; x += 2)
        {
            ಭಿನ್ನ B0 = ಭಿನ್ನ(src0[(x+0)*NCHANNELS+0]);
            ಭಿನ್ನ G0 = ಭಿನ್ನ(src0[(x+0)*NCHANNELS+1]);
            ಭಿನ್ನ R0 = ಭಿನ್ನ(src0[(x+0)*NCHANNELS+2]);

            ಭಿನ್ನ B1 = ಭಿನ್ನ(src0[(x+1)*NCHANNELS+0]);
            ಭಿನ್ನ G1 = ಭಿನ್ನ(src0[(x+1)*NCHANNELS+1]);
            ಭಿನ್ನ R1 = ಭಿನ್ನ(src0[(x+1)*NCHANNELS+2]);

            ಭಿನ್ನ B2 = ಭಿನ್ನ(src1[(x+0)*NCHANNELS+0]);
            ಭಿನ್ನ G2 = ಭಿನ್ನ(src1[(x+0)*NCHANNELS+1]);
            ಭಿನ್ನ R2 = ಭಿನ್ನ(src1[(x+0)*NCHANNELS+2]);

            ಭಿನ್ನ B3 = ಭಿನ್ನ(src1[(x+1)*NCHANNELS+0]);
            ಭಿನ್ನ G3 = ಭಿನ್ನ(src1[(x+1)*NCHANNELS+1]);
            ಭಿನ್ನ R3 = ಭಿನ್ನ(src1[(x+1)*NCHANNELS+2]);

            ಭಿನ್ನ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ಭಿನ್ನ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ಭಿನ್ನ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ಭಿನ್ನ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ಭಿನ್ನ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ಭಿನ್ನ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned ವರ್ಣ>(Y0);
            dstY0[x+1] = saturate_cast<unsigned ವರ್ಣ>(Y1);
            dstY1[x+0] = saturate_cast<unsigned ವರ್ಣ>(Y2);
            dstY1[x+1] = saturate_cast<unsigned ವರ್ಣ>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned ವರ್ಣ>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned ವರ್ಣ>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_yv12_to_bgr(ಧ್ರುವ VAImage& image, ಧ್ರುವ unsigned ವರ್ಣ* buffer, Mat& bgr)
{
    ಧ್ರುವ ಭಿನ್ನ d1 = 16.0f;
    ಧ್ರುವ ಭಿನ್ನ d2 = 128.0f;

    ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ಅಂಕೆ_planes, (size_t)3, "");

    ಧ್ರುವ size_t srcOffsetY = image.offsets[0];
    ಧ್ರುವ size_t srcOffsetV = image.offsets[1];
    ಧ್ರುವ size_t srcOffsetU = image.offsets[2];

    ಧ್ರುವ size_t srcStepY = image.pitches[0];
    ಧ್ರುವ size_t srcStepU = image.pitches[1];
    ಧ್ರುವ size_t srcStepV = image.pitches[2];

    ಧ್ರುವ size_t dstStep = bgr.step;

    ಧ್ರುವ unsigned ವರ್ಣ* srcY_ = buffer + srcOffsetY;
    ಧ್ರುವ unsigned ವರ್ಣ* srcV_ = buffer + srcOffsetV;
    ಧ್ರುವ unsigned ವರ್ಣ* srcU_ = buffer + srcOffsetU;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < bgr.rows; y += 2)
    {
        ಧ್ರುವ unsigned ವರ್ಣ* srcY0 = srcY_ + (srcStepY) * y;
        ಧ್ರುವ unsigned ವರ್ಣ* srcY1 = srcY0 + srcStepY;

        ಧ್ರುವ unsigned ವರ್ಣ* srcV = srcV_ + (srcStepV) * y / 2;
        ಧ್ರುವ unsigned ವರ್ಣ* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned ವರ್ಣ* dst0 = bgr.data + (dstStep) * y;
        unsigned ವರ್ಣ* dst1 = dst0 + dstStep;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < bgr.cols; x += 2)
        {
            ಭಿನ್ನ Y0 = ಭಿನ್ನ(srcY0[x+0]);
            ಭಿನ್ನ Y1 = ಭಿನ್ನ(srcY0[x+1]);
            ಭಿನ್ನ Y2 = ಭಿನ್ನ(srcY1[x+0]);
            ಭಿನ್ನ Y3 = ಭಿನ್ನ(srcY1[x+1]);

            ಭಿನ್ನ U = ಭಿನ್ನ(srcU[x/2]) - d2;
            ಭಿನ್ನ V = ಭಿನ್ನ(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ಭಿನ್ನ ruv = coeffs[4]*V;
            ಭಿನ್ನ guv = coeffs[3]*V + coeffs[2]*U;
            ಭಿನ್ನ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ವರ್ಣ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ವರ್ಣ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ವರ್ಣ>(Y3 + ruv);
        }
    }
}

ಸ್ಥಿರ ಶೂನ್ಯ copy_convert_bgr_to_yv12(ಧ್ರುವ VAImage& image, ಧ್ರುವ Mat& bgr, unsigned ವರ್ಣ* buffer)
{
    ಧ್ರುವ ಭಿನ್ನ d1 = 16.0f;
    ಧ್ರುವ ಭಿನ್ನ d2 = 128.0f;

    ಸ್ಥಿರ ಧ್ರುವ ಭಿನ್ನ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ಅಂಕೆ_planes, (size_t)3, "");

    ಧ್ರುವ size_t dstOffsetY = image.offsets[0];
    ಧ್ರುವ size_t dstOffsetV = image.offsets[1];
    ಧ್ರುವ size_t dstOffsetU = image.offsets[2];

    ಧ್ರುವ size_t dstStepY = image.pitches[0];
    ಧ್ರುವ size_t dstStepU = image.pitches[1];
    ಧ್ರುವ size_t dstStepV = image.pitches[2];

    unsigned ವರ್ಣ* dstY_ = buffer + dstOffsetY;
    unsigned ವರ್ಣ* dstV_ = buffer + dstOffsetV;
    unsigned ವರ್ಣ* dstU_ = buffer + dstOffsetU;

    ಧ್ರುವ size_t srcStep = bgr.step;

    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ y = 0; y < bgr.rows; y += 2)
    {
        unsigned ವರ್ಣ* dstY0 = dstY_ + (dstStepY) * y;
        unsigned ವರ್ಣ* dstY1 = dstY0 + dstStepY;

        unsigned ವರ್ಣ* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned ವರ್ಣ* dstU = dstU_ + (dstStepU) * y / 2;

        ಧ್ರುವ unsigned ವರ್ಣ* src0 = bgr.data + (srcStep) * y;
        ಧ್ರುವ unsigned ವರ್ಣ* src1 = src0 + srcStep;

        ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ x = 0; x < bgr.cols; x += 2)
        {
            ಭಿನ್ನ B0 = ಭಿನ್ನ(src0[(x+0)*NCHANNELS+0]);
            ಭಿನ್ನ G0 = ಭಿನ್ನ(src0[(x+0)*NCHANNELS+1]);
            ಭಿನ್ನ R0 = ಭಿನ್ನ(src0[(x+0)*NCHANNELS+2]);

            ಭಿನ್ನ B1 = ಭಿನ್ನ(src0[(x+1)*NCHANNELS+0]);
            ಭಿನ್ನ G1 = ಭಿನ್ನ(src0[(x+1)*NCHANNELS+1]);
            ಭಿನ್ನ R1 = ಭಿನ್ನ(src0[(x+1)*NCHANNELS+2]);

            ಭಿನ್ನ B2 = ಭಿನ್ನ(src1[(x+0)*NCHANNELS+0]);
            ಭಿನ್ನ G2 = ಭಿನ್ನ(src1[(x+0)*NCHANNELS+1]);
            ಭಿನ್ನ R2 = ಭಿನ್ನ(src1[(x+0)*NCHANNELS+2]);

            ಭಿನ್ನ B3 = ಭಿನ್ನ(src1[(x+1)*NCHANNELS+0]);
            ಭಿನ್ನ G3 = ಭಿನ್ನ(src1[(x+1)*NCHANNELS+1]);
            ಭಿನ್ನ R3 = ಭಿನ್ನ(src1[(x+1)*NCHANNELS+2]);

            ಭಿನ್ನ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ಭಿನ್ನ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ಭಿನ್ನ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ಭಿನ್ನ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ಭಿನ್ನ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ಭಿನ್ನ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned ವರ್ಣ>(Y0);
            dstY0[x+1] = saturate_cast<unsigned ವರ್ಣ>(Y1);
            dstY1[x+0] = saturate_cast<unsigned ವರ್ಣ>(Y2);
            dstY1[x+1] = saturate_cast<unsigned ವರ್ಣ>(Y3);

            dstU[x/2] = saturate_cast<unsigned ವರ್ಣ>(U);
            dstV[x/2] = saturate_cast<unsigned ವರ್ಣ>(V);
        }
    }
}
#endif // HAVE_VA

ಶೂನ್ಯ convertToVASurface(VADisplay display, ಒಳಹರಿವು_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#ಯದಿ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ಅನ್ಯಥಾ  // !HAVE_VA

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ stype = CV_8UC3;

    ಪೂರ್ಣಾಙ್ಕ srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ರಿಕ್ತ, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    ಶೂನ್ಯ* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ರಿಕ್ತ, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    ತಾರ್ಕಿಕ isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ರಿಕ್ತ, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (ಶೂನ್ಯ*)display << "(surface)");
    ಯದಿ (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ರಿಕ್ತ, ರಿಕ್ತ);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ಯದಿ (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (ಪೂರ್ಣಾಙ್ಕ)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ರಿಕ್ತ, ರಿಕ್ತ);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ಅನ್ಯಥಾ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        ತಾರ್ಕಿಕ indirect_buffer = ಅಸತ್ಯ;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        ಯದಿ (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = ಸತ್ಯ;
            ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_formats = va_Max_ಅಂಕೆ_Image_Formats(display);
            ಯದಿ (ಅಂಕೆ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ಅಂಕೆ_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &ಅಂಕೆ_formats);
            ಯದಿ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            ಪ್ರತಿ (auto &fmt : fmt_list){
                ಯದಿ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ಭಙ್ಗ;
                }
            }
            ಯದಿ (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            ಯದಿ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned ವರ್ಣ* buffer = 0;
        status = vaMapBuffer(display, image.buf, (ಶೂನ್ಯ **)&buffer);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ಯದಿ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        ಯದಿ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        ಅನ್ಯಥಾ
            CV_Check((ಪೂರ್ಣಾಙ್ಕ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        ಯದಿ (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            ಯದಿ (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

ಶೂನ್ಯ convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#ಯದಿ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ಅನ್ಯಥಾ  // !HAVE_VA

    ಧ್ರುವ ಪೂರ್ಣಾಙ್ಕ dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    ಯದಿ (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ರಿಕ್ತ, ರಿಕ್ತ);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ಯದಿ (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (ಪೂರ್ಣಾಙ್ಕ)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ರಿಕ್ತ, ರಿಕ್ತ);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        ಯದಿ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ಅನ್ಯಥಾ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        ಯದಿ (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ_formats = va_Max_ಅಂಕೆ_Image_Formats(display);
            ಯದಿ (ಅಂಕೆ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ಅಂಕೆ_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &ಅಂಕೆ_formats);
            ಯದಿ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            ಪ್ರತಿ (auto &fmt : fmt_list){
                ಯದಿ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ಭಙ್ಗ;
                }
            }
            ಯದಿ (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            ಯದಿ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            ಯದಿ (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned ವರ್ಣ* buffer = 0;
        status = vaMapBuffer(display, image.buf, (ಶೂನ್ಯ **)&buffer);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ಯದಿ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        ಯದಿ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        ಅನ್ಯಥಾ
            CV_Check((ಪೂರ್ಣಾಙ್ಕ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        ಯದಿ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
