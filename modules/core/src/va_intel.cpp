// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#அந்தர்பாவ "precomp.hpp"

#அந்தர்பாவ <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#இல்லையெனில்  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using நாமாகாஶ cv;

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
#இல்லையெனில்
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#அந்தர்பாவ "va_wrapper.impl.hpp"
#இல்லையெனில்
நாமாகாஶ cv { நாமாகாஶ detail {
நிலையான வெறுமை init_libva() { /* nothing */ }
}}  // namespace
#endif
using நாமாகாஶ cv::detail;
#endif

நாமாகாஶ cv { நாமாகாஶ va_intel {

#ifdef HAVE_VA_INTEL

வகுப்பு VAAPIInterop : பொது ocl::Context::UserContext
{
பொது:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        எனில் (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    மெய்நிகர் ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

நாமாகாஶ ocl {

Context& initializeContextFromVA(VADisplay display, தருக்க tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#எனில் !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#இல்லையெனில்  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    எனில் (tryInterop)
    {
        cl_uint இலக்கம்_Platforms;
        cl_int status = clGetPlatformIDs(0, வெற்று, &இலக்கம்_Platforms);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        எனில் (இலக்கம்_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(இலக்கம்_Platforms);
        status = clGetPlatformIDs(இலக்கம்_Platforms, &platforms[0], வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        முழு found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        க்கு (முழு i = 0; i < (முழு)இலக்கம்_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            எனில் ((வெறுமை*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == வெற்று)
            {
                தொடர்;
            }

            // Query device list

            cl_uint இலக்கம்_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, வெற்று, &இலக்கம்_Devices);
            எனில் ((status != CL_SUCCESS) || !(இலக்கம்_Devices > 0))
                தொடர்;
            இலக்கம்_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, இலக்கம்_Devices, &device, வெற்று);
            எனில் (status != CL_SUCCESS)
                தொடர்;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, இலக்கம்_Devices, &device, வெற்று, வெற்று, &status);
            எனில் (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            இல்லையெனில்
            {
                found = i;
                நிறுத்து;
            }
        }

        எனில் (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            முயல்
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            பிடி (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                எறி;
            }
            clExecCtx.bind();
            திரும்பு const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        திரும்பு ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
நிலையான தருக்க ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, முழு step, முழு cols, முழு rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    எனில் (k.empty())
        திரும்பு பொய்;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    திரும்பு k.run(2, globalsize, 0, பொய்);
}

நிலையான தருக்க ocl_convert_bgr_to_nv12(cl_mem clBuffer, முழு step, முழு cols, முழு rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    எனில் (k.empty())
        திரும்பு பொய்;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    திரும்பு k.run(2, globalsize, 0, பொய்);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#எனில் defined(HAVE_VA)
மாறிலி முழு NCHANNELS = 3;

நிலையான வெறுமை copy_convert_nv12_to_bgr(மாறிலி VAImage& image, மாறிலி unsigned எழுத்து* buffer, Mat& bgr)
{
    மாறிலி தசம d1 = 16.0f;
    மாறிலி தசம d2 = 128.0f;

    நிலையான மாறிலி தசம coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    மாறிலி size_t srcOffsetY = image.offsets[0];
    மாறிலி size_t srcOffsetUV = image.offsets[1];

    மாறிலி size_t srcStepY = image.pitches[0];
    மாறிலி size_t srcStepUV = image.pitches[1];

    மாறிலி size_t dstStep = bgr.step;

    மாறிலி unsigned எழுத்து* srcY0 = buffer + srcOffsetY;
    மாறிலி unsigned எழுத்து* srcUV = buffer + srcOffsetUV;

    unsigned எழுத்து* dst0 = bgr.data;

    க்கு (முழு y = 0; y < bgr.rows; y += 2)
    {
        மாறிலி unsigned எழுத்து* srcY1 = srcY0 + srcStepY;
        unsigned எழுத்து *dst1 = dst0 + dstStep;

        க்கு (முழு x = 0; x < bgr.cols; x += 2)
        {
            தசம Y0 = தசம(srcY0[x+0]);
            தசம Y1 = தசம(srcY0[x+1]);
            தசம Y2 = தசம(srcY1[x+0]);
            தசம Y3 = தசம(srcY1[x+1]);

            தசம U = தசம(srcUV[2*(x/2)+0]) - d2;
            தசம V = தசம(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            தசம ruv = coeffs[4]*V;
            தசம guv = coeffs[3]*V + coeffs[2]*U;
            தசம buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

நிலையான வெறுமை copy_convert_bgr_to_nv12(மாறிலி VAImage& image, மாறிலி Mat& bgr, unsigned எழுத்து* buffer)
{
    மாறிலி தசம d1 = 16.0f;
    மாறிலி தசம d2 = 128.0f;

    நிலையான மாறிலி தசம coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    மாறிலி size_t dstOffsetY = image.offsets[0];
    மாறிலி size_t dstOffsetUV = image.offsets[1];

    மாறிலி size_t dstStepY = image.pitches[0];
    மாறிலி size_t dstStepUV = image.pitches[1];

    மாறிலி size_t srcStep = bgr.step;

    மாறிலி unsigned எழுத்து* src0 = bgr.data;

    unsigned எழுத்து* dstY0 = buffer + dstOffsetY;
    unsigned எழுத்து* dstUV = buffer + dstOffsetUV;

    க்கு (முழு y = 0; y < bgr.rows; y += 2)
    {
        மாறிலி unsigned எழுத்து *src1 = src0 + srcStep;
        unsigned எழுத்து* dstY1 = dstY0 + dstStepY;

        க்கு (முழு x = 0; x < bgr.cols; x += 2)
        {
            தசம B0 = தசம(src0[(x+0)*NCHANNELS+0]);
            தசம G0 = தசம(src0[(x+0)*NCHANNELS+1]);
            தசம R0 = தசம(src0[(x+0)*NCHANNELS+2]);

            தசம B1 = தசம(src0[(x+1)*NCHANNELS+0]);
            தசம G1 = தசம(src0[(x+1)*NCHANNELS+1]);
            தசம R1 = தசம(src0[(x+1)*NCHANNELS+2]);

            தசம B2 = தசம(src1[(x+0)*NCHANNELS+0]);
            தசம G2 = தசம(src1[(x+0)*NCHANNELS+1]);
            தசம R2 = தசம(src1[(x+0)*NCHANNELS+2]);

            தசம B3 = தசம(src1[(x+1)*NCHANNELS+0]);
            தசம G3 = தசம(src1[(x+1)*NCHANNELS+1]);
            தசம R3 = தசம(src1[(x+1)*NCHANNELS+2]);

            தசம Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            தசம Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            தசம Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            தசம Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            தசம U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            தசம V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned எழுத்து>(Y0);
            dstY0[x+1] = saturate_cast<unsigned எழுத்து>(Y1);
            dstY1[x+0] = saturate_cast<unsigned எழுத்து>(Y2);
            dstY1[x+1] = saturate_cast<unsigned எழுத்து>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned எழுத்து>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned எழுத்து>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


நிலையான வெறுமை copy_convert_yv12_to_bgr(மாறிலி VAImage& image, மாறிலி unsigned எழுத்து* buffer, Mat& bgr)
{
    மாறிலி தசம d1 = 16.0f;
    மாறிலி தசம d2 = 128.0f;

    நிலையான மாறிலி தசம coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.இலக்கம்_planes, (size_t)3, "");

    மாறிலி size_t srcOffsetY = image.offsets[0];
    மாறிலி size_t srcOffsetV = image.offsets[1];
    மாறிலி size_t srcOffsetU = image.offsets[2];

    மாறிலி size_t srcStepY = image.pitches[0];
    மாறிலி size_t srcStepU = image.pitches[1];
    மாறிலி size_t srcStepV = image.pitches[2];

    மாறிலி size_t dstStep = bgr.step;

    மாறிலி unsigned எழுத்து* srcY_ = buffer + srcOffsetY;
    மாறிலி unsigned எழுத்து* srcV_ = buffer + srcOffsetV;
    மாறிலி unsigned எழுத்து* srcU_ = buffer + srcOffsetU;

    க்கு (முழு y = 0; y < bgr.rows; y += 2)
    {
        மாறிலி unsigned எழுத்து* srcY0 = srcY_ + (srcStepY) * y;
        மாறிலி unsigned எழுத்து* srcY1 = srcY0 + srcStepY;

        மாறிலி unsigned எழுத்து* srcV = srcV_ + (srcStepV) * y / 2;
        மாறிலி unsigned எழுத்து* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned எழுத்து* dst0 = bgr.data + (dstStep) * y;
        unsigned எழுத்து* dst1 = dst0 + dstStep;

        க்கு (முழு x = 0; x < bgr.cols; x += 2)
        {
            தசம Y0 = தசம(srcY0[x+0]);
            தசம Y1 = தசம(srcY0[x+1]);
            தசம Y2 = தசம(srcY1[x+0]);
            தசம Y3 = தசம(srcY1[x+1]);

            தசம U = தசம(srcU[x/2]) - d2;
            தசம V = தசம(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            தசம ruv = coeffs[4]*V;
            தசம guv = coeffs[3]*V + coeffs[2]*U;
            தசம buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned எழுத்து>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned எழுத்து>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned எழுத்து>(Y3 + ruv);
        }
    }
}

நிலையான வெறுமை copy_convert_bgr_to_yv12(மாறிலி VAImage& image, மாறிலி Mat& bgr, unsigned எழுத்து* buffer)
{
    மாறிலி தசம d1 = 16.0f;
    மாறிலி தசம d2 = 128.0f;

    நிலையான மாறிலி தசம coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.இலக்கம்_planes, (size_t)3, "");

    மாறிலி size_t dstOffsetY = image.offsets[0];
    மாறிலி size_t dstOffsetV = image.offsets[1];
    மாறிலி size_t dstOffsetU = image.offsets[2];

    மாறிலி size_t dstStepY = image.pitches[0];
    மாறிலி size_t dstStepU = image.pitches[1];
    மாறிலி size_t dstStepV = image.pitches[2];

    unsigned எழுத்து* dstY_ = buffer + dstOffsetY;
    unsigned எழுத்து* dstV_ = buffer + dstOffsetV;
    unsigned எழுத்து* dstU_ = buffer + dstOffsetU;

    மாறிலி size_t srcStep = bgr.step;

    க்கு (முழு y = 0; y < bgr.rows; y += 2)
    {
        unsigned எழுத்து* dstY0 = dstY_ + (dstStepY) * y;
        unsigned எழுத்து* dstY1 = dstY0 + dstStepY;

        unsigned எழுத்து* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned எழுத்து* dstU = dstU_ + (dstStepU) * y / 2;

        மாறிலி unsigned எழுத்து* src0 = bgr.data + (srcStep) * y;
        மாறிலி unsigned எழுத்து* src1 = src0 + srcStep;

        க்கு (முழு x = 0; x < bgr.cols; x += 2)
        {
            தசம B0 = தசம(src0[(x+0)*NCHANNELS+0]);
            தசம G0 = தசம(src0[(x+0)*NCHANNELS+1]);
            தசம R0 = தசம(src0[(x+0)*NCHANNELS+2]);

            தசம B1 = தசம(src0[(x+1)*NCHANNELS+0]);
            தசம G1 = தசம(src0[(x+1)*NCHANNELS+1]);
            தசம R1 = தசம(src0[(x+1)*NCHANNELS+2]);

            தசம B2 = தசம(src1[(x+0)*NCHANNELS+0]);
            தசம G2 = தசம(src1[(x+0)*NCHANNELS+1]);
            தசம R2 = தசம(src1[(x+0)*NCHANNELS+2]);

            தசம B3 = தசம(src1[(x+1)*NCHANNELS+0]);
            தசம G3 = தசம(src1[(x+1)*NCHANNELS+1]);
            தசம R3 = தசம(src1[(x+1)*NCHANNELS+2]);

            தசம Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            தசம Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            தசம Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            தசம Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            தசம U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            தசம V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned எழுத்து>(Y0);
            dstY0[x+1] = saturate_cast<unsigned எழுத்து>(Y1);
            dstY1[x+0] = saturate_cast<unsigned எழுத்து>(Y2);
            dstY1[x+1] = saturate_cast<unsigned எழுத்து>(Y3);

            dstU[x/2] = saturate_cast<unsigned எழுத்து>(U);
            dstV[x/2] = saturate_cast<unsigned எழுத்து>(V);
        }
    }
}
#endif // HAVE_VA

வெறுமை convertToVASurface(VADisplay display, உள்ளீடு_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#எனில் !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#இல்லையெனில்  // !HAVE_VA

    மாறிலி முழு stype = CV_8UC3;

    முழு srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(வெற்று, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    வெறுமை* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(வெற்று, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    தருக்க isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(வெற்று, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (வெறுமை*)display << "(surface)");
    எனில் (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        எனில் (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (முழு)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    இல்லையெனில்
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        தருக்க indirect_buffer = பொய்;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        எனில் (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = உண்மை;
            முழு இலக்கம்_formats = va_Max_இலக்கம்_Image_Formats(display);
            எனில் (இலக்கம்_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(இலக்கம்_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &இலக்கம்_formats);
            எனில் (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            க்கு (auto &fmt : fmt_list){
                எனில் (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    நிறுத்து;
                }
            }
            எனில் (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            எனில் (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned எழுத்து* buffer = 0;
        status = vaMapBuffer(display, image.buf, (வெறுமை **)&buffer);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        எனில் (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        எனில் (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        இல்லையெனில்
            CV_Check((முழு)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        எனில் (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            எனில் (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

வெறுமை convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#எனில் !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#இல்லையெனில்  // !HAVE_VA

    மாறிலி முழு dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    எனில் (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        எனில் (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (முழு)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, வெற்று, வெற்று);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        எனில் (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    இல்லையெனில்
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        எனில் (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            முழு இலக்கம்_formats = va_Max_இலக்கம்_Image_Formats(display);
            எனில் (இலக்கம்_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(இலக்கம்_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &இலக்கம்_formats);
            எனில் (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            க்கு (auto &fmt : fmt_list){
                எனில் (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    நிறுத்து;
                }
            }
            எனில் (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            எனில் (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            எனில் (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned எழுத்து* buffer = 0;
        status = vaMapBuffer(display, image.buf, (வெறுமை **)&buffer);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        எனில் (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        எனில் (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        இல்லையெனில்
            CV_Check((முழு)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        எனில் (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
