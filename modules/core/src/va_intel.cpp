// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#অন্তর্ভাব "precomp.hpp"

#অন্তর্ভাব <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#অন্যথা  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using নামাকাশ cv;

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
#অন্যথা
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#অন্তর্ভাব "va_wrapper.impl.hpp"
#অন্যথা
নামাকাশ cv { নামাকাশ detail {
স্থির শূন্য init_libva() { /* nothing */ }
}}  // namespace
#endif
using নামাকাশ cv::detail;
#endif

নামাকাশ cv { নামাকাশ va_intel {

#ifdef HAVE_VA_INTEL

বর্গ VAAPIInterop : সার্বজনিক ocl::Context::UserContext
{
সার্বজনিক:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        যদি (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    আভাস ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

নামাকাশ ocl {

Context& initializeContextFromVA(VADisplay display, তার্কিক tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#যদি !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#অন্যথা  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    যদি (tryInterop)
    {
        cl_uint অঙ্ক_Platforms;
        cl_int status = clGetPlatformIDs(0, রিক্ত, &অঙ্ক_Platforms);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        যদি (অঙ্ক_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(অঙ্ক_Platforms);
        status = clGetPlatformIDs(অঙ্ক_Platforms, &platforms[0], রিক্ত);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        পূর্ণাঙ্ক found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        প্রতি (পূর্ণাঙ্ক i = 0; i < (পূর্ণাঙ্ক)অঙ্ক_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            যদি ((শূন্য*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == রিক্ত)
            {
                অনুবর্ত;
            }

            // Query device list

            cl_uint অঙ্ক_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, রিক্ত, &অঙ্ক_Devices);
            যদি ((status != CL_SUCCESS) || !(অঙ্ক_Devices > 0))
                অনুবর্ত;
            অঙ্ক_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, অঙ্ক_Devices, &device, রিক্ত);
            যদি (status != CL_SUCCESS)
                অনুবর্ত;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, অঙ্ক_Devices, &device, রিক্ত, রিক্ত, &status);
            যদি (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            অন্যথা
            {
                found = i;
                ভঙ্গ;
            }
        }

        যদি (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            যত্ন
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            গৃহাণ (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ক্ষিপ;
            }
            clExecCtx.bind();
            প্রত্যাগম const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        প্রত্যাগম ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
স্থির তার্কিক ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, পূর্ণাঙ্ক step, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    প্রত্যাগম k.run(2, globalsize, 0, অসত্য);
}

স্থির তার্কিক ocl_convert_bgr_to_nv12(cl_mem clBuffer, পূর্ণাঙ্ক step, পূর্ণাঙ্ক cols, পূর্ণাঙ্ক rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    যদি (k.empty())
        প্রত্যাগম অসত্য;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    প্রত্যাগম k.run(2, globalsize, 0, অসত্য);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#যদি defined(HAVE_VA)
ধ্রুব পূর্ণাঙ্ক NCHANNELS = 3;

স্থির শূন্য copy_convert_nv12_to_bgr(ধ্রুব VAImage& image, ধ্রুব unsigned বর্ণ* buffer, Mat& bgr)
{
    ধ্রুব ভিন্ন d1 = 16.0f;
    ধ্রুব ভিন্ন d2 = 128.0f;

    স্থির ধ্রুব ভিন্ন coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ধ্রুব size_t srcOffsetY = image.offsets[0];
    ধ্রুব size_t srcOffsetUV = image.offsets[1];

    ধ্রুব size_t srcStepY = image.pitches[0];
    ধ্রুব size_t srcStepUV = image.pitches[1];

    ধ্রুব size_t dstStep = bgr.step;

    ধ্রুব unsigned বর্ণ* srcY0 = buffer + srcOffsetY;
    ধ্রুব unsigned বর্ণ* srcUV = buffer + srcOffsetUV;

    unsigned বর্ণ* dst0 = bgr.data;

    প্রতি (পূর্ণাঙ্ক y = 0; y < bgr.rows; y += 2)
    {
        ধ্রুব unsigned বর্ণ* srcY1 = srcY0 + srcStepY;
        unsigned বর্ণ *dst1 = dst0 + dstStep;

        প্রতি (পূর্ণাঙ্ক x = 0; x < bgr.cols; x += 2)
        {
            ভিন্ন Y0 = ভিন্ন(srcY0[x+0]);
            ভিন্ন Y1 = ভিন্ন(srcY0[x+1]);
            ভিন্ন Y2 = ভিন্ন(srcY1[x+0]);
            ভিন্ন Y3 = ভিন্ন(srcY1[x+1]);

            ভিন্ন U = ভিন্ন(srcUV[2*(x/2)+0]) - d2;
            ভিন্ন V = ভিন্ন(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ভিন্ন ruv = coeffs[4]*V;
            ভিন্ন guv = coeffs[3]*V + coeffs[2]*U;
            ভিন্ন buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

স্থির শূন্য copy_convert_bgr_to_nv12(ধ্রুব VAImage& image, ধ্রুব Mat& bgr, unsigned বর্ণ* buffer)
{
    ধ্রুব ভিন্ন d1 = 16.0f;
    ধ্রুব ভিন্ন d2 = 128.0f;

    স্থির ধ্রুব ভিন্ন coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ধ্রুব size_t dstOffsetY = image.offsets[0];
    ধ্রুব size_t dstOffsetUV = image.offsets[1];

    ধ্রুব size_t dstStepY = image.pitches[0];
    ধ্রুব size_t dstStepUV = image.pitches[1];

    ধ্রুব size_t srcStep = bgr.step;

    ধ্রুব unsigned বর্ণ* src0 = bgr.data;

    unsigned বর্ণ* dstY0 = buffer + dstOffsetY;
    unsigned বর্ণ* dstUV = buffer + dstOffsetUV;

    প্রতি (পূর্ণাঙ্ক y = 0; y < bgr.rows; y += 2)
    {
        ধ্রুব unsigned বর্ণ *src1 = src0 + srcStep;
        unsigned বর্ণ* dstY1 = dstY0 + dstStepY;

        প্রতি (পূর্ণাঙ্ক x = 0; x < bgr.cols; x += 2)
        {
            ভিন্ন B0 = ভিন্ন(src0[(x+0)*NCHANNELS+0]);
            ভিন্ন G0 = ভিন্ন(src0[(x+0)*NCHANNELS+1]);
            ভিন্ন R0 = ভিন্ন(src0[(x+0)*NCHANNELS+2]);

            ভিন্ন B1 = ভিন্ন(src0[(x+1)*NCHANNELS+0]);
            ভিন্ন G1 = ভিন্ন(src0[(x+1)*NCHANNELS+1]);
            ভিন্ন R1 = ভিন্ন(src0[(x+1)*NCHANNELS+2]);

            ভিন্ন B2 = ভিন্ন(src1[(x+0)*NCHANNELS+0]);
            ভিন্ন G2 = ভিন্ন(src1[(x+0)*NCHANNELS+1]);
            ভিন্ন R2 = ভিন্ন(src1[(x+0)*NCHANNELS+2]);

            ভিন্ন B3 = ভিন্ন(src1[(x+1)*NCHANNELS+0]);
            ভিন্ন G3 = ভিন্ন(src1[(x+1)*NCHANNELS+1]);
            ভিন্ন R3 = ভিন্ন(src1[(x+1)*NCHANNELS+2]);

            ভিন্ন Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ভিন্ন Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ভিন্ন Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ভিন্ন Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ভিন্ন U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ভিন্ন V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned বর্ণ>(Y0);
            dstY0[x+1] = saturate_cast<unsigned বর্ণ>(Y1);
            dstY1[x+0] = saturate_cast<unsigned বর্ণ>(Y2);
            dstY1[x+1] = saturate_cast<unsigned বর্ণ>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned বর্ণ>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned বর্ণ>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


স্থির শূন্য copy_convert_yv12_to_bgr(ধ্রুব VAImage& image, ধ্রুব unsigned বর্ণ* buffer, Mat& bgr)
{
    ধ্রুব ভিন্ন d1 = 16.0f;
    ধ্রুব ভিন্ন d2 = 128.0f;

    স্থির ধ্রুব ভিন্ন coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.অঙ্ক_planes, (size_t)3, "");

    ধ্রুব size_t srcOffsetY = image.offsets[0];
    ধ্রুব size_t srcOffsetV = image.offsets[1];
    ধ্রুব size_t srcOffsetU = image.offsets[2];

    ধ্রুব size_t srcStepY = image.pitches[0];
    ধ্রুব size_t srcStepU = image.pitches[1];
    ধ্রুব size_t srcStepV = image.pitches[2];

    ধ্রুব size_t dstStep = bgr.step;

    ধ্রুব unsigned বর্ণ* srcY_ = buffer + srcOffsetY;
    ধ্রুব unsigned বর্ণ* srcV_ = buffer + srcOffsetV;
    ধ্রুব unsigned বর্ণ* srcU_ = buffer + srcOffsetU;

    প্রতি (পূর্ণাঙ্ক y = 0; y < bgr.rows; y += 2)
    {
        ধ্রুব unsigned বর্ণ* srcY0 = srcY_ + (srcStepY) * y;
        ধ্রুব unsigned বর্ণ* srcY1 = srcY0 + srcStepY;

        ধ্রুব unsigned বর্ণ* srcV = srcV_ + (srcStepV) * y / 2;
        ধ্রুব unsigned বর্ণ* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned বর্ণ* dst0 = bgr.data + (dstStep) * y;
        unsigned বর্ণ* dst1 = dst0 + dstStep;

        প্রতি (পূর্ণাঙ্ক x = 0; x < bgr.cols; x += 2)
        {
            ভিন্ন Y0 = ভিন্ন(srcY0[x+0]);
            ভিন্ন Y1 = ভিন্ন(srcY0[x+1]);
            ভিন্ন Y2 = ভিন্ন(srcY1[x+0]);
            ভিন্ন Y3 = ভিন্ন(srcY1[x+1]);

            ভিন্ন U = ভিন্ন(srcU[x/2]) - d2;
            ভিন্ন V = ভিন্ন(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            ভিন্ন ruv = coeffs[4]*V;
            ভিন্ন guv = coeffs[3]*V + coeffs[2]*U;
            ভিন্ন buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned বর্ণ>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned বর্ণ>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned বর্ণ>(Y3 + ruv);
        }
    }
}

স্থির শূন্য copy_convert_bgr_to_yv12(ধ্রুব VAImage& image, ধ্রুব Mat& bgr, unsigned বর্ণ* buffer)
{
    ধ্রুব ভিন্ন d1 = 16.0f;
    ধ্রুব ভিন্ন d2 = 128.0f;

    স্থির ধ্রুব ভিন্ন coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.অঙ্ক_planes, (size_t)3, "");

    ধ্রুব size_t dstOffsetY = image.offsets[0];
    ধ্রুব size_t dstOffsetV = image.offsets[1];
    ধ্রুব size_t dstOffsetU = image.offsets[2];

    ধ্রুব size_t dstStepY = image.pitches[0];
    ধ্রুব size_t dstStepU = image.pitches[1];
    ধ্রুব size_t dstStepV = image.pitches[2];

    unsigned বর্ণ* dstY_ = buffer + dstOffsetY;
    unsigned বর্ণ* dstV_ = buffer + dstOffsetV;
    unsigned বর্ণ* dstU_ = buffer + dstOffsetU;

    ধ্রুব size_t srcStep = bgr.step;

    প্রতি (পূর্ণাঙ্ক y = 0; y < bgr.rows; y += 2)
    {
        unsigned বর্ণ* dstY0 = dstY_ + (dstStepY) * y;
        unsigned বর্ণ* dstY1 = dstY0 + dstStepY;

        unsigned বর্ণ* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned বর্ণ* dstU = dstU_ + (dstStepU) * y / 2;

        ধ্রুব unsigned বর্ণ* src0 = bgr.data + (srcStep) * y;
        ধ্রুব unsigned বর্ণ* src1 = src0 + srcStep;

        প্রতি (পূর্ণাঙ্ক x = 0; x < bgr.cols; x += 2)
        {
            ভিন্ন B0 = ভিন্ন(src0[(x+0)*NCHANNELS+0]);
            ভিন্ন G0 = ভিন্ন(src0[(x+0)*NCHANNELS+1]);
            ভিন্ন R0 = ভিন্ন(src0[(x+0)*NCHANNELS+2]);

            ভিন্ন B1 = ভিন্ন(src0[(x+1)*NCHANNELS+0]);
            ভিন্ন G1 = ভিন্ন(src0[(x+1)*NCHANNELS+1]);
            ভিন্ন R1 = ভিন্ন(src0[(x+1)*NCHANNELS+2]);

            ভিন্ন B2 = ভিন্ন(src1[(x+0)*NCHANNELS+0]);
            ভিন্ন G2 = ভিন্ন(src1[(x+0)*NCHANNELS+1]);
            ভিন্ন R2 = ভিন্ন(src1[(x+0)*NCHANNELS+2]);

            ভিন্ন B3 = ভিন্ন(src1[(x+1)*NCHANNELS+0]);
            ভিন্ন G3 = ভিন্ন(src1[(x+1)*NCHANNELS+1]);
            ভিন্ন R3 = ভিন্ন(src1[(x+1)*NCHANNELS+2]);

            ভিন্ন Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            ভিন্ন Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            ভিন্ন Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            ভিন্ন Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            ভিন্ন U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            ভিন্ন V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned বর্ণ>(Y0);
            dstY0[x+1] = saturate_cast<unsigned বর্ণ>(Y1);
            dstY1[x+0] = saturate_cast<unsigned বর্ণ>(Y2);
            dstY1[x+1] = saturate_cast<unsigned বর্ণ>(Y3);

            dstU[x/2] = saturate_cast<unsigned বর্ণ>(U);
            dstV[x/2] = saturate_cast<unsigned বর্ণ>(V);
        }
    }
}
#endif // HAVE_VA

শূন্য convertToVASurface(VADisplay display, প্রবেশ_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#যদি !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#অন্যথা  // !HAVE_VA

    ধ্রুব পূর্ণাঙ্ক stype = CV_8UC3;

    পূর্ণাঙ্ক srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(রিক্ত, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    শূন্য* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(রিক্ত, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    তার্কিক isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(রিক্ত, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (শূন্য*)display << "(surface)");
    যদি (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, রিক্ত, রিক্ত);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        যদি (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (পূর্ণাঙ্ক)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, রিক্ত, রিক্ত);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    অন্যথা
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        তার্কিক indirect_buffer = অসত্য;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        যদি (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = সত্য;
            পূর্ণাঙ্ক অঙ্ক_formats = va_Max_অঙ্ক_Image_Formats(display);
            যদি (অঙ্ক_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(অঙ্ক_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &অঙ্ক_formats);
            যদি (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            প্রতি (auto &fmt : fmt_list){
                যদি (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ভঙ্গ;
                }
            }
            যদি (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            যদি (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned বর্ণ* buffer = 0;
        status = vaMapBuffer(display, image.buf, (শূন্য **)&buffer);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        যদি (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        যদি (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        অন্যথা
            CV_Check((পূর্ণাঙ্ক)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        যদি (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            যদি (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

শূন্য convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#যদি !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#অন্যথা  // !HAVE_VA

    ধ্রুব পূর্ণাঙ্ক dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    যদি (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, রিক্ত, রিক্ত);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        যদি (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (পূর্ণাঙ্ক)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, রিক্ত, রিক্ত);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    অন্যথা
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        যদি (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            পূর্ণাঙ্ক অঙ্ক_formats = va_Max_অঙ্ক_Image_Formats(display);
            যদি (অঙ্ক_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(অঙ্ক_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &অঙ্ক_formats);
            যদি (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            প্রতি (auto &fmt : fmt_list){
                যদি (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ভঙ্গ;
                }
            }
            যদি (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            যদি (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            যদি (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned বর্ণ* buffer = 0;
        status = vaMapBuffer(display, image.buf, (শূন্য **)&buffer);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        যদি (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        যদি (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        অন্যথা
            CV_Check((পূর্ণাঙ্ক)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
