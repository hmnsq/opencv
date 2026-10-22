// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#അന്തര്ഭാവ "precomp.hpp"

#അന്തര്ഭാവ <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#അല്ലെങ്കിൽ  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using നാമാകാശ cv;

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
#അല്ലെങ്കിൽ
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#അന്തര്ഭാവ "va_wrapper.impl.hpp"
#അല്ലെങ്കിൽ
നാമാകാശ cv { നാമാകാശ detail {
സ്ഥിതം ശൂന്യം init_libva() { /* nothing */ }
}}  // namespace
#endif
using നാമാകാശ cv::detail;
#endif

നാമാകാശ cv { നാമാകാശ va_intel {

#ifdef HAVE_VA_INTEL

വർഗ്ഗം VAAPIInterop : പൊതു ocl::Context::UserContext
{
പൊതു:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        എങ്കിൽ (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    മായാ ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

നാമാകാശ ocl {

Context& initializeContextFromVA(VADisplay display, തർക്കം tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#എങ്കിൽ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#അല്ലെങ്കിൽ  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    എങ്കിൽ (tryInterop)
    {
        cl_uint അക്കം_Platforms;
        cl_int status = clGetPlatformIDs(0, ശൂന്യ, &അക്കം_Platforms);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        എങ്കിൽ (അക്കം_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(അക്കം_Platforms);
        status = clGetPlatformIDs(അക്കം_Platforms, &platforms[0], ശൂന്യ);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        പൂർണ്ണസംഖ്യ found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < (പൂർണ്ണസംഖ്യ)അക്കം_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            എങ്കിൽ ((ശൂന്യം*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ശൂന്യ)
            {
                തുടരുക;
            }

            // Query device list

            cl_uint അക്കം_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ശൂന്യ, &അക്കം_Devices);
            എങ്കിൽ ((status != CL_SUCCESS) || !(അക്കം_Devices > 0))
                തുടരുക;
            അക്കം_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, അക്കം_Devices, &device, ശൂന്യ);
            എങ്കിൽ (status != CL_SUCCESS)
                തുടരുക;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, അക്കം_Devices, &device, ശൂന്യ, ശൂന്യ, &status);
            എങ്കിൽ (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            അല്ലെങ്കിൽ
            {
                found = i;
                നിർത്തുക;
            }
        }

        എങ്കിൽ (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            ശ്രമിക്കുക
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            പിടിക്കുക (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                എറിയുക;
            }
            clExecCtx.bind();
            തിരികെ const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        തിരികെ ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
സ്ഥിതം തർക്കം ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, പൂർണ്ണസംഖ്യ step, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    തിരികെ k.run(2, globalsize, 0, അസത്യം);
}

സ്ഥിതം തർക്കം ocl_convert_bgr_to_nv12(cl_mem clBuffer, പൂർണ്ണസംഖ്യ step, പൂർണ്ണസംഖ്യ cols, പൂർണ്ണസംഖ്യ rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    എങ്കിൽ (k.empty())
        തിരികെ അസത്യം;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    തിരികെ k.run(2, globalsize, 0, അസത്യം);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#എങ്കിൽ defined(HAVE_VA)
സ്ഥിരം പൂർണ്ണസംഖ്യ NCHANNELS = 3;

സ്ഥിതം ശൂന്യം copy_convert_nv12_to_bgr(സ്ഥിരം VAImage& image, സ്ഥിരം unsigned അക്ഷരം* buffer, Mat& bgr)
{
    സ്ഥിരം ദശാംശം d1 = 16.0f;
    സ്ഥിരം ദശാംശം d2 = 128.0f;

    സ്ഥിതം സ്ഥിരം ദശാംശം coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    സ്ഥിരം size_t srcOffsetY = image.offsets[0];
    സ്ഥിരം size_t srcOffsetUV = image.offsets[1];

    സ്ഥിരം size_t srcStepY = image.pitches[0];
    സ്ഥിരം size_t srcStepUV = image.pitches[1];

    സ്ഥിരം size_t dstStep = bgr.step;

    സ്ഥിരം unsigned അക്ഷരം* srcY0 = buffer + srcOffsetY;
    സ്ഥിരം unsigned അക്ഷരം* srcUV = buffer + srcOffsetUV;

    unsigned അക്ഷരം* dst0 = bgr.data;

    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < bgr.rows; y += 2)
    {
        സ്ഥിരം unsigned അക്ഷരം* srcY1 = srcY0 + srcStepY;
        unsigned അക്ഷരം *dst1 = dst0 + dstStep;

        വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < bgr.cols; x += 2)
        {
            ദശാംശം Y0 = ദശാംശം(srcY0[x+0]);
            ദശാംശം Y1 = ദശാംശം(srcY0[x+1]);
            ദശാംശം Y2 = ദശാംശം(srcY1[x+0]);
            ദശാംശം Y3 = ദശാംശം(srcY1[x+1]);

            ദശാംശം U = ദശാംശം(srcUV[2*(x/2)+0]) - d2;
            ദശാംശം V = ദശാംശം(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ദശാംശം ruv = coeffs[4]*V;
            ദശാംശം guv = coeffs[3]*V + coeffs[2]*U;
            ദശാംശം buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

സ്ഥിതം ശൂന്യം copy_convert_bgr_to_nv12(സ്ഥിരം VAImage& image, സ്ഥിരം Mat& bgr, unsigned അക്ഷരം* buffer)
{
    സ്ഥിരം ദശാംശം d1 = 16.0f;
    സ്ഥിരം ദശാംശം d2 = 128.0f;

    സ്ഥിതം സ്ഥിരം ദശാംശം coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    സ്ഥിരം size_t dstOffsetY = image.offsets[0];
    സ്ഥിരം size_t dstOffsetUV = image.offsets[1];

    സ്ഥിരം size_t dstStepY = image.pitches[0];
    സ്ഥിരം size_t dstStepUV = image.pitches[1];

    സ്ഥിരം size_t srcStep = bgr.step;

    സ്ഥിരം unsigned അക്ഷരം* src0 = bgr.data;

    unsigned അക്ഷരം* dstY0 = buffer + dstOffsetY;
    unsigned അക്ഷരം* dstUV = buffer + dstOffsetUV;

    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < bgr.rows; y += 2)
    {
        സ്ഥിരം unsigned അക്ഷരം *src1 = src0 + srcStep;
        unsigned അക്ഷരം* dstY1 = dstY0 + dstStepY;

        വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < bgr.cols; x += 2)
        {
            ദശാംശം B0 = ദശാംശം(src0[(x+0)*NCHANNELS+0]);
            ദശാംശം G0 = ദശാംശം(src0[(x+0)*NCHANNELS+1]);
            ദശാംശം R0 = ദശാംശം(src0[(x+0)*NCHANNELS+2]);

            ദശാംശം B1 = ദശാംശം(src0[(x+1)*NCHANNELS+0]);
            ദശാംശം G1 = ദശാംശം(src0[(x+1)*NCHANNELS+1]);
            ദശാംശം R1 = ദശാംശം(src0[(x+1)*NCHANNELS+2]);

            ദശാംശം B2 = ദശാംശം(src1[(x+0)*NCHANNELS+0]);
            ദശാംശം G2 = ദശാംശം(src1[(x+0)*NCHANNELS+1]);
            ദശാംശം R2 = ദശാംശം(src1[(x+0)*NCHANNELS+2]);

            ദശാംശം B3 = ദശാംശം(src1[(x+1)*NCHANNELS+0]);
            ദശാംശം G3 = ദശാംശം(src1[(x+1)*NCHANNELS+1]);
            ദശാംശം R3 = ദശാംശം(src1[(x+1)*NCHANNELS+2]);

            ദശാംശം Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ദശാംശം Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ദശാംശം Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ദശാംശം Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ദശാംശം U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ദശാംശം V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned അക്ഷരം>(Y0);
            dstY0[x+1] = saturate_cast<unsigned അക്ഷരം>(Y1);
            dstY1[x+0] = saturate_cast<unsigned അക്ഷരം>(Y2);
            dstY1[x+1] = saturate_cast<unsigned അക്ഷരം>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned അക്ഷരം>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned അക്ഷരം>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


സ്ഥിതം ശൂന്യം copy_convert_yv12_to_bgr(സ്ഥിരം VAImage& image, സ്ഥിരം unsigned അക്ഷരം* buffer, Mat& bgr)
{
    സ്ഥിരം ദശാംശം d1 = 16.0f;
    സ്ഥിരം ദശാംശം d2 = 128.0f;

    സ്ഥിതം സ്ഥിരം ദശാംശം coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.അക്കം_planes, (size_t)3, "");

    സ്ഥിരം size_t srcOffsetY = image.offsets[0];
    സ്ഥിരം size_t srcOffsetV = image.offsets[1];
    സ്ഥിരം size_t srcOffsetU = image.offsets[2];

    സ്ഥിരം size_t srcStepY = image.pitches[0];
    സ്ഥിരം size_t srcStepU = image.pitches[1];
    സ്ഥിരം size_t srcStepV = image.pitches[2];

    സ്ഥിരം size_t dstStep = bgr.step;

    സ്ഥിരം unsigned അക്ഷരം* srcY_ = buffer + srcOffsetY;
    സ്ഥിരം unsigned അക്ഷരം* srcV_ = buffer + srcOffsetV;
    സ്ഥിരം unsigned അക്ഷരം* srcU_ = buffer + srcOffsetU;

    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < bgr.rows; y += 2)
    {
        സ്ഥിരം unsigned അക്ഷരം* srcY0 = srcY_ + (srcStepY) * y;
        സ്ഥിരം unsigned അക്ഷരം* srcY1 = srcY0 + srcStepY;

        സ്ഥിരം unsigned അക്ഷരം* srcV = srcV_ + (srcStepV) * y / 2;
        സ്ഥിരം unsigned അക്ഷരം* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned അക്ഷരം* dst0 = bgr.data + (dstStep) * y;
        unsigned അക്ഷരം* dst1 = dst0 + dstStep;

        വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < bgr.cols; x += 2)
        {
            ദശാംശം Y0 = ദശാംശം(srcY0[x+0]);
            ദശാംശം Y1 = ദശാംശം(srcY0[x+1]);
            ദശാംശം Y2 = ദശാംശം(srcY1[x+0]);
            ദശാംശം Y3 = ദശാംശം(srcY1[x+1]);

            ദശാംശം U = ദശാംശം(srcU[x/2]) - d2;
            ദശാംശം V = ദശാംശം(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ദശാംശം ruv = coeffs[4]*V;
            ദശാംശം guv = coeffs[3]*V + coeffs[2]*U;
            ദശാംശം buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned അക്ഷരം>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned അക്ഷരം>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned അക്ഷരം>(Y3 + ruv);
        }
    }
}

സ്ഥിതം ശൂന്യം copy_convert_bgr_to_yv12(സ്ഥിരം VAImage& image, സ്ഥിരം Mat& bgr, unsigned അക്ഷരം* buffer)
{
    സ്ഥിരം ദശാംശം d1 = 16.0f;
    സ്ഥിരം ദശാംശം d2 = 128.0f;

    സ്ഥിതം സ്ഥിരം ദശാംശം coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.അക്കം_planes, (size_t)3, "");

    സ്ഥിരം size_t dstOffsetY = image.offsets[0];
    സ്ഥിരം size_t dstOffsetV = image.offsets[1];
    സ്ഥിരം size_t dstOffsetU = image.offsets[2];

    സ്ഥിരം size_t dstStepY = image.pitches[0];
    സ്ഥിരം size_t dstStepU = image.pitches[1];
    സ്ഥിരം size_t dstStepV = image.pitches[2];

    unsigned അക്ഷരം* dstY_ = buffer + dstOffsetY;
    unsigned അക്ഷരം* dstV_ = buffer + dstOffsetV;
    unsigned അക്ഷരം* dstU_ = buffer + dstOffsetU;

    സ്ഥിരം size_t srcStep = bgr.step;

    വേണ്ടി (പൂർണ്ണസംഖ്യ y = 0; y < bgr.rows; y += 2)
    {
        unsigned അക്ഷരം* dstY0 = dstY_ + (dstStepY) * y;
        unsigned അക്ഷരം* dstY1 = dstY0 + dstStepY;

        unsigned അക്ഷരം* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned അക്ഷരം* dstU = dstU_ + (dstStepU) * y / 2;

        സ്ഥിരം unsigned അക്ഷരം* src0 = bgr.data + (srcStep) * y;
        സ്ഥിരം unsigned അക്ഷരം* src1 = src0 + srcStep;

        വേണ്ടി (പൂർണ്ണസംഖ്യ x = 0; x < bgr.cols; x += 2)
        {
            ദശാംശം B0 = ദശാംശം(src0[(x+0)*NCHANNELS+0]);
            ദശാംശം G0 = ദശാംശം(src0[(x+0)*NCHANNELS+1]);
            ദശാംശം R0 = ദശാംശം(src0[(x+0)*NCHANNELS+2]);

            ദശാംശം B1 = ദശാംശം(src0[(x+1)*NCHANNELS+0]);
            ദശാംശം G1 = ദശാംശം(src0[(x+1)*NCHANNELS+1]);
            ദശാംശം R1 = ദശാംശം(src0[(x+1)*NCHANNELS+2]);

            ദശാംശം B2 = ദശാംശം(src1[(x+0)*NCHANNELS+0]);
            ദശാംശം G2 = ദശാംശം(src1[(x+0)*NCHANNELS+1]);
            ദശാംശം R2 = ദശാംശം(src1[(x+0)*NCHANNELS+2]);

            ദശാംശം B3 = ദശാംശം(src1[(x+1)*NCHANNELS+0]);
            ദശാംശം G3 = ദശാംശം(src1[(x+1)*NCHANNELS+1]);
            ദശാംശം R3 = ദശാംശം(src1[(x+1)*NCHANNELS+2]);

            ദശാംശം Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ദശാംശം Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ദശാംശം Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ദശാംശം Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ദശാംശം U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ദശാംശം V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned അക്ഷരം>(Y0);
            dstY0[x+1] = saturate_cast<unsigned അക്ഷരം>(Y1);
            dstY1[x+0] = saturate_cast<unsigned അക്ഷരം>(Y2);
            dstY1[x+1] = saturate_cast<unsigned അക്ഷരം>(Y3);

            dstU[x/2] = saturate_cast<unsigned അക്ഷരം>(U);
            dstV[x/2] = saturate_cast<unsigned അക്ഷരം>(V);
        }
    }
}
#endif // HAVE_VA

ശൂന്യം convertToVASurface(VADisplay display, നിവേശം_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#എങ്കിൽ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#അല്ലെങ്കിൽ  // !HAVE_VA

    സ്ഥിരം പൂർണ്ണസംഖ്യ stype = CV_8UC3;

    പൂർണ്ണസംഖ്യ srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ശൂന്യ, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    ശൂന്യം* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ശൂന്യ, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    തർക്കം isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ശൂന്യ, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (ശൂന്യം*)display << "(surface)");
    എങ്കിൽ (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ശൂന്യ, ശൂന്യ);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        എങ്കിൽ (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (പൂർണ്ണസംഖ്യ)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ശൂന്യ, ശൂന്യ);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    അല്ലെങ്കിൽ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        തർക്കം indirect_buffer = അസത്യം;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        എങ്കിൽ (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = സത്യം;
            പൂർണ്ണസംഖ്യ അക്കം_formats = va_Max_അക്കം_Image_Formats(display);
            എങ്കിൽ (അക്കം_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(അക്കം_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &അക്കം_formats);
            എങ്കിൽ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            വേണ്ടി (auto &fmt : fmt_list){
                എങ്കിൽ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    നിർത്തുക;
                }
            }
            എങ്കിൽ (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            എങ്കിൽ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned അക്ഷരം* buffer = 0;
        status = vaMapBuffer(display, image.buf, (ശൂന്യം **)&buffer);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        എങ്കിൽ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        എങ്കിൽ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        അല്ലെങ്കിൽ
            CV_Check((പൂർണ്ണസംഖ്യ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        എങ്കിൽ (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            എങ്കിൽ (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

ശൂന്യം convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#എങ്കിൽ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#അല്ലെങ്കിൽ  // !HAVE_VA

    സ്ഥിരം പൂർണ്ണസംഖ്യ dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    എങ്കിൽ (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ശൂന്യ, ശൂന്യ);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        എങ്കിൽ (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (പൂർണ്ണസംഖ്യ)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ശൂന്യ, ശൂന്യ);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        എങ്കിൽ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    അല്ലെങ്കിൽ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        എങ്കിൽ (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            പൂർണ്ണസംഖ്യ അക്കം_formats = va_Max_അക്കം_Image_Formats(display);
            എങ്കിൽ (അക്കം_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(അക്കം_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &അക്കം_formats);
            എങ്കിൽ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            വേണ്ടി (auto &fmt : fmt_list){
                എങ്കിൽ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    നിർത്തുക;
                }
            }
            എങ്കിൽ (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            എങ്കിൽ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            എങ്കിൽ (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned അക്ഷരം* buffer = 0;
        status = vaMapBuffer(display, image.buf, (ശൂന്യം **)&buffer);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        എങ്കിൽ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        എങ്കിൽ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        അല്ലെങ്കിൽ
            CV_Check((പൂർണ്ണസംഖ്യ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        എങ്കിൽ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
