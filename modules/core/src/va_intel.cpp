// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ଅନ୍ତର୍ଭାଵ "precomp.hpp"

#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#ଅନ୍ୟଥା  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using ନାମାକାଶ cv;

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
#ଅନ୍ୟଥା
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#ଅନ୍ତର୍ଭାଵ "va_wrapper.impl.hpp"
#ଅନ୍ୟଥା
ନାମାକାଶ cv { ନାମାକାଶ detail {
ସ୍ଥିର ଶୂନ୍ୟ init_libva() { /* nothing */ }
}}  // namespace
#endif
using ନାମାକାଶ cv::detail;
#endif

ନାମାକାଶ cv { ନାମାକାଶ va_intel {

#ifdef HAVE_VA_INTEL

ଶ୍ରେଣୀ VAAPIInterop : ସର୍ବସାଧାରଣ ocl::Context::UserContext
{
ସର୍ବସାଧାରଣ:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        ଯଦି (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    ଆଭାସୀ ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

ନାମାକାଶ ocl {

Context& initializeContextFromVA(VADisplay display, ତାର୍କିକ tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#ଯଦି !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ଅନ୍ୟଥା  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    ଯଦି (tryInterop)
    {
        cl_uint ଅଙ୍କ_Platforms;
        cl_int status = clGetPlatformIDs(0, ଖାଲି, &ଅଙ୍କ_Platforms);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        ଯଦି (ଅଙ୍କ_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(ଅଙ୍କ_Platforms);
        status = clGetPlatformIDs(ଅଙ୍କ_Platforms, &platforms[0], ଖାଲି);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        ପୂର୍ଣ୍ଣସଂଖ୍ୟା found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଅଙ୍କ_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            ଯଦି ((ଶୂନ୍ୟ*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ଖାଲି)
            {
                ଜାରି;
            }

            // Query device list

            cl_uint ଅଙ୍କ_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ଖାଲି, &ଅଙ୍କ_Devices);
            ଯଦି ((status != CL_SUCCESS) || !(ଅଙ୍କ_Devices > 0))
                ଜାରି;
            ଅଙ୍କ_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, ଅଙ୍କ_Devices, &device, ଖାଲି);
            ଯଦି (status != CL_SUCCESS)
                ଜାରି;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, ଅଙ୍କ_Devices, &device, ଖାଲି, ଖାଲି, &status);
            ଯଦି (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            ଅନ୍ୟଥା
            {
                found = i;
                ଭାଙ୍ଗ;
            }
        }

        ଯଦି (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            ଚେଷ୍ଟା
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            ଧର (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ଫିଙ୍ଗ;
            }
            clExecCtx.bind();
            ଫେରନ୍ତୁ const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        ଫେରନ୍ତୁ ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
ସ୍ଥିର ତାର୍କିକ ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ଫେରନ୍ତୁ k.run(2, globalsize, 0, ମିଥ୍ୟା);
}

ସ୍ଥିର ତାର୍କିକ ocl_convert_bgr_to_nv12(cl_mem clBuffer, ପୂର୍ଣ୍ଣସଂଖ୍ୟା step, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cols, ପୂର୍ଣ୍ଣସଂଖ୍ୟା rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ଯଦି (k.empty())
        ଫେରନ୍ତୁ ମିଥ୍ୟା;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ଫେରନ୍ତୁ k.run(2, globalsize, 0, ମିଥ୍ୟା);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#ଯଦି defined(HAVE_VA)
ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା NCHANNELS = 3;

ସ୍ଥିର ଶୂନ୍ୟ copy_convert_nv12_to_bgr(ଅଚଳ VAImage& image, ଅଚଳ unsigned ଅକ୍ଷର* buffer, Mat& bgr)
{
    ଅଚଳ ଦଶମିକ d1 = 16.0f;
    ଅଚଳ ଦଶମିକ d2 = 128.0f;

    ସ୍ଥିର ଅଚଳ ଦଶମିକ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ଅଚଳ size_t srcOffsetY = image.offsets[0];
    ଅଚଳ size_t srcOffsetUV = image.offsets[1];

    ଅଚଳ size_t srcStepY = image.pitches[0];
    ଅଚଳ size_t srcStepUV = image.pitches[1];

    ଅଚଳ size_t dstStep = bgr.step;

    ଅଚଳ unsigned ଅକ୍ଷର* srcY0 = buffer + srcOffsetY;
    ଅଚଳ unsigned ଅକ୍ଷର* srcUV = buffer + srcOffsetUV;

    unsigned ଅକ୍ଷର* dst0 = bgr.data;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < bgr.rows; y += 2)
    {
        ଅଚଳ unsigned ଅକ୍ଷର* srcY1 = srcY0 + srcStepY;
        unsigned ଅକ୍ଷର *dst1 = dst0 + dstStep;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < bgr.cols; x += 2)
        {
            ଦଶମିକ Y0 = ଦଶମିକ(srcY0[x+0]);
            ଦଶମିକ Y1 = ଦଶମିକ(srcY0[x+1]);
            ଦଶମିକ Y2 = ଦଶମିକ(srcY1[x+0]);
            ଦଶମିକ Y3 = ଦଶମିକ(srcY1[x+1]);

            ଦଶମିକ U = ଦଶମିକ(srcUV[2*(x/2)+0]) - d2;
            ଦଶମିକ V = ଦଶମିକ(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ଦଶମିକ ruv = coeffs[4]*V;
            ଦଶମିକ guv = coeffs[3]*V + coeffs[2]*U;
            ଦଶମିକ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

ସ୍ଥିର ଶୂନ୍ୟ copy_convert_bgr_to_nv12(ଅଚଳ VAImage& image, ଅଚଳ Mat& bgr, unsigned ଅକ୍ଷର* buffer)
{
    ଅଚଳ ଦଶମିକ d1 = 16.0f;
    ଅଚଳ ଦଶମିକ d2 = 128.0f;

    ସ୍ଥିର ଅଚଳ ଦଶମିକ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ଅଚଳ size_t dstOffsetY = image.offsets[0];
    ଅଚଳ size_t dstOffsetUV = image.offsets[1];

    ଅଚଳ size_t dstStepY = image.pitches[0];
    ଅଚଳ size_t dstStepUV = image.pitches[1];

    ଅଚଳ size_t srcStep = bgr.step;

    ଅଚଳ unsigned ଅକ୍ଷର* src0 = bgr.data;

    unsigned ଅକ୍ଷର* dstY0 = buffer + dstOffsetY;
    unsigned ଅକ୍ଷର* dstUV = buffer + dstOffsetUV;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < bgr.rows; y += 2)
    {
        ଅଚଳ unsigned ଅକ୍ଷର *src1 = src0 + srcStep;
        unsigned ଅକ୍ଷର* dstY1 = dstY0 + dstStepY;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < bgr.cols; x += 2)
        {
            ଦଶମିକ B0 = ଦଶମିକ(src0[(x+0)*NCHANNELS+0]);
            ଦଶମିକ G0 = ଦଶମିକ(src0[(x+0)*NCHANNELS+1]);
            ଦଶମିକ R0 = ଦଶମିକ(src0[(x+0)*NCHANNELS+2]);

            ଦଶମିକ B1 = ଦଶମିକ(src0[(x+1)*NCHANNELS+0]);
            ଦଶମିକ G1 = ଦଶମିକ(src0[(x+1)*NCHANNELS+1]);
            ଦଶମିକ R1 = ଦଶମିକ(src0[(x+1)*NCHANNELS+2]);

            ଦଶମିକ B2 = ଦଶମିକ(src1[(x+0)*NCHANNELS+0]);
            ଦଶମିକ G2 = ଦଶମିକ(src1[(x+0)*NCHANNELS+1]);
            ଦଶମିକ R2 = ଦଶମିକ(src1[(x+0)*NCHANNELS+2]);

            ଦଶମିକ B3 = ଦଶମିକ(src1[(x+1)*NCHANNELS+0]);
            ଦଶମିକ G3 = ଦଶମିକ(src1[(x+1)*NCHANNELS+1]);
            ଦଶମିକ R3 = ଦଶମିକ(src1[(x+1)*NCHANNELS+2]);

            ଦଶମିକ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ଦଶମିକ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ଦଶମିକ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ଦଶମିକ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ଦଶମିକ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ଦଶମିକ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y0);
            dstY0[x+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y1);
            dstY1[x+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y2);
            dstY1[x+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned ଅକ୍ଷର>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned ଅକ୍ଷର>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


ସ୍ଥିର ଶୂନ୍ୟ copy_convert_yv12_to_bgr(ଅଚଳ VAImage& image, ଅଚଳ unsigned ଅକ୍ଷର* buffer, Mat& bgr)
{
    ଅଚଳ ଦଶମିକ d1 = 16.0f;
    ଅଚଳ ଦଶମିକ d2 = 128.0f;

    ସ୍ଥିର ଅଚଳ ଦଶମିକ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ଅଙ୍କ_planes, (size_t)3, "");

    ଅଚଳ size_t srcOffsetY = image.offsets[0];
    ଅଚଳ size_t srcOffsetV = image.offsets[1];
    ଅଚଳ size_t srcOffsetU = image.offsets[2];

    ଅଚଳ size_t srcStepY = image.pitches[0];
    ଅଚଳ size_t srcStepU = image.pitches[1];
    ଅଚଳ size_t srcStepV = image.pitches[2];

    ଅଚଳ size_t dstStep = bgr.step;

    ଅଚଳ unsigned ଅକ୍ଷର* srcY_ = buffer + srcOffsetY;
    ଅଚଳ unsigned ଅକ୍ଷର* srcV_ = buffer + srcOffsetV;
    ଅଚଳ unsigned ଅକ୍ଷର* srcU_ = buffer + srcOffsetU;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < bgr.rows; y += 2)
    {
        ଅଚଳ unsigned ଅକ୍ଷର* srcY0 = srcY_ + (srcStepY) * y;
        ଅଚଳ unsigned ଅକ୍ଷର* srcY1 = srcY0 + srcStepY;

        ଅଚଳ unsigned ଅକ୍ଷର* srcV = srcV_ + (srcStepV) * y / 2;
        ଅଚଳ unsigned ଅକ୍ଷର* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned ଅକ୍ଷର* dst0 = bgr.data + (dstStep) * y;
        unsigned ଅକ୍ଷର* dst1 = dst0 + dstStep;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < bgr.cols; x += 2)
        {
            ଦଶମିକ Y0 = ଦଶମିକ(srcY0[x+0]);
            ଦଶମିକ Y1 = ଦଶମିକ(srcY0[x+1]);
            ଦଶମିକ Y2 = ଦଶମିକ(srcY1[x+0]);
            ଦଶମିକ Y3 = ଦଶମିକ(srcY1[x+1]);

            ଦଶମିକ U = ଦଶମିକ(srcU[x/2]) - d2;
            ଦଶମିକ V = ଦଶମିକ(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ଦଶମିକ ruv = coeffs[4]*V;
            ଦଶମିକ guv = coeffs[3]*V + coeffs[2]*U;
            ଦଶମିକ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned ଅକ୍ଷର>(Y3 + ruv);
        }
    }
}

ସ୍ଥିର ଶୂନ୍ୟ copy_convert_bgr_to_yv12(ଅଚଳ VAImage& image, ଅଚଳ Mat& bgr, unsigned ଅକ୍ଷର* buffer)
{
    ଅଚଳ ଦଶମିକ d1 = 16.0f;
    ଅଚଳ ଦଶମିକ d2 = 128.0f;

    ସ୍ଥିର ଅଚଳ ଦଶମିକ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.ଅଙ୍କ_planes, (size_t)3, "");

    ଅଚଳ size_t dstOffsetY = image.offsets[0];
    ଅଚଳ size_t dstOffsetV = image.offsets[1];
    ଅଚଳ size_t dstOffsetU = image.offsets[2];

    ଅଚଳ size_t dstStepY = image.pitches[0];
    ଅଚଳ size_t dstStepU = image.pitches[1];
    ଅଚଳ size_t dstStepV = image.pitches[2];

    unsigned ଅକ୍ଷର* dstY_ = buffer + dstOffsetY;
    unsigned ଅକ୍ଷର* dstV_ = buffer + dstOffsetV;
    unsigned ଅକ୍ଷର* dstU_ = buffer + dstOffsetU;

    ଅଚଳ size_t srcStep = bgr.step;

    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା y = 0; y < bgr.rows; y += 2)
    {
        unsigned ଅକ୍ଷର* dstY0 = dstY_ + (dstStepY) * y;
        unsigned ଅକ୍ଷର* dstY1 = dstY0 + dstStepY;

        unsigned ଅକ୍ଷର* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned ଅକ୍ଷର* dstU = dstU_ + (dstStepU) * y / 2;

        ଅଚଳ unsigned ଅକ୍ଷର* src0 = bgr.data + (srcStep) * y;
        ଅଚଳ unsigned ଅକ୍ଷର* src1 = src0 + srcStep;

        ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା x = 0; x < bgr.cols; x += 2)
        {
            ଦଶମିକ B0 = ଦଶମିକ(src0[(x+0)*NCHANNELS+0]);
            ଦଶମିକ G0 = ଦଶମିକ(src0[(x+0)*NCHANNELS+1]);
            ଦଶମିକ R0 = ଦଶମିକ(src0[(x+0)*NCHANNELS+2]);

            ଦଶମିକ B1 = ଦଶମିକ(src0[(x+1)*NCHANNELS+0]);
            ଦଶମିକ G1 = ଦଶମିକ(src0[(x+1)*NCHANNELS+1]);
            ଦଶମିକ R1 = ଦଶମିକ(src0[(x+1)*NCHANNELS+2]);

            ଦଶମିକ B2 = ଦଶମିକ(src1[(x+0)*NCHANNELS+0]);
            ଦଶମିକ G2 = ଦଶମିକ(src1[(x+0)*NCHANNELS+1]);
            ଦଶମିକ R2 = ଦଶମିକ(src1[(x+0)*NCHANNELS+2]);

            ଦଶମିକ B3 = ଦଶମିକ(src1[(x+1)*NCHANNELS+0]);
            ଦଶମିକ G3 = ଦଶମିକ(src1[(x+1)*NCHANNELS+1]);
            ଦଶମିକ R3 = ଦଶମିକ(src1[(x+1)*NCHANNELS+2]);

            ଦଶମିକ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ଦଶମିକ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ଦଶମିକ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ଦଶମିକ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ଦଶମିକ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ଦଶମିକ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y0);
            dstY0[x+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y1);
            dstY1[x+0] = saturate_cast<unsigned ଅକ୍ଷର>(Y2);
            dstY1[x+1] = saturate_cast<unsigned ଅକ୍ଷର>(Y3);

            dstU[x/2] = saturate_cast<unsigned ଅକ୍ଷର>(U);
            dstV[x/2] = saturate_cast<unsigned ଅକ୍ଷର>(V);
        }
    }
}
#endif // HAVE_VA

ଶୂନ୍ୟ convertToVASurface(VADisplay display, ନିବେଶ_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#ଯଦି !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ଅନ୍ୟଥା  // !HAVE_VA

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା stype = CV_8UC3;

    ପୂର୍ଣ୍ଣସଂଖ୍ୟା srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ଖାଲି, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    ଶୂନ୍ୟ* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ଖାଲି, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    ତାର୍କିକ isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ଖାଲି, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (ଶୂନ୍ୟ*)display << "(surface)");
    ଯଦି (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ଖାଲି, ଖାଲି);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ଯଦି (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ଖାଲି, ଖାଲି);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ଅନ୍ୟଥା
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        ତାର୍କିକ indirect_buffer = ମିଥ୍ୟା;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        ଯଦି (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = ସତ୍ୟ;
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_formats = va_Max_ଅଙ୍କ_Image_Formats(display);
            ଯଦି (ଅଙ୍କ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ଅଙ୍କ_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &ଅଙ୍କ_formats);
            ଯଦି (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            ପାଇଁ (auto &fmt : fmt_list){
                ଯଦି (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ଭାଙ୍ଗ;
                }
            }
            ଯଦି (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            ଯଦି (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned ଅକ୍ଷର* buffer = 0;
        status = vaMapBuffer(display, image.buf, (ଶୂନ୍ୟ **)&buffer);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ଯଦି (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        ଯଦି (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        ଅନ୍ୟଥା
            CV_Check((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        ଯଦି (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            ଯଦି (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

ଶୂନ୍ୟ convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#ଯଦି !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ଅନ୍ୟଥା  // !HAVE_VA

    ଅଚଳ ପୂର୍ଣ୍ଣସଂଖ୍ୟା dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    ଯଦି (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ଖାଲି, ଖାଲି);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ଯଦି (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ଖାଲି, ଖାଲି);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        ଯଦି (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ଅନ୍ୟଥା
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        ଯଦି (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ_formats = va_Max_ଅଙ୍କ_Image_Formats(display);
            ଯଦି (ଅଙ୍କ_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(ଅଙ୍କ_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &ଅଙ୍କ_formats);
            ଯଦି (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            ପାଇଁ (auto &fmt : fmt_list){
                ଯଦି (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ଭାଙ୍ଗ;
                }
            }
            ଯଦି (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            ଯଦି (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            ଯଦି (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned ଅକ୍ଷର* buffer = 0;
        status = vaMapBuffer(display, image.buf, (ଶୂନ୍ୟ **)&buffer);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ଯଦି (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        ଯଦି (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        ଅନ୍ୟଥା
            CV_Check((ପୂର୍ଣ୍ଣସଂଖ୍ୟା)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        ଯଦି (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
