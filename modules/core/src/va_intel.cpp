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

শ্রেণী VAAPIInterop : সর্বজনীন ocl::Context::UserContext
{
সর্বজনীন:
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
    আভাসী ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

নামাকাশ ocl {

Context& initializeContextFromVA(VADisplay display, যৌক্তিক tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#যদি !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#অন্যথা  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    যদি (tryInterop)
    {
        cl_uint অঙ্ক_Platforms;
        cl_int status = clGetPlatformIDs(0, ফাঁকা, &অঙ্ক_Platforms);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        যদি (অঙ্ক_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(অঙ্ক_Platforms);
        status = clGetPlatformIDs(অঙ্ক_Platforms, &platforms[0], ফাঁকা);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        পূর্ণসংখ্যা found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        জন্য (পূর্ণসংখ্যা i = 0; i < (পূর্ণসংখ্যা)অঙ্ক_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            যদি ((শূন্য*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ফাঁকা)
            {
                চালু;
            }

            // Query device list

            cl_uint অঙ্ক_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ফাঁকা, &অঙ্ক_Devices);
            যদি ((status != CL_SUCCESS) || !(অঙ্ক_Devices > 0))
                চালু;
            অঙ্ক_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, অঙ্ক_Devices, &device, ফাঁকা);
            যদি (status != CL_SUCCESS)
                চালু;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, অঙ্ক_Devices, &device, ফাঁকা, ফাঁকা, &status);
            যদি (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            অন্যথা
            {
                found = i;
                বিরতি;
            }
        }

        যদি (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            চেষ্টা
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            ধরো (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ছোড়ো;
            }
            clExecCtx.bind();
            ফেরত const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        ফেরত ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
স্থির যৌক্তিক ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_বাফার, পূর্ণসংখ্যা step, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    যদি (k.empty())
        ফেরত মিথ্যা;

    k.args(clImageY, clImageUV, cl_বাফার, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ফেরত k.run(2, globalsize, 0, মিথ্যা);
}

স্থির যৌক্তিক ocl_convert_bgr_to_nv12(cl_mem cl_বাফার, পূর্ণসংখ্যা step, পূর্ণসংখ্যা cols, পূর্ণসংখ্যা rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    যদি (k.empty())
        ফেরত মিথ্যা;

    k.args(cl_বাফার, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ফেরত k.run(2, globalsize, 0, মিথ্যা);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#যদি defined(HAVE_VA)
ধ্রুবক পূর্ণসংখ্যা NCHANNELS = 3;

স্থির শূন্য copy_convert_nv12_to_bgr(ধ্রুবক VAImage& image, ধ্রুবক unsigned অক্ষর* বাফার, Mat& bgr)
{
    ধ্রুবক দশমিক d1 = 16.0f;
    ধ্রুবক দশমিক d2 = 128.0f;

    স্থির ধ্রুবক দশমিক coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ধ্রুবক size_t srcOffsetY = image.offsets[0];
    ধ্রুবক size_t srcOffsetUV = image.offsets[1];

    ধ্রুবক size_t srcStepY = image.pitches[0];
    ধ্রুবক size_t srcStepUV = image.pitches[1];

    ধ্রুবক size_t dstStep = bgr.step;

    ধ্রুবক unsigned অক্ষর* srcY0 = বাফার + srcOffsetY;
    ধ্রুবক unsigned অক্ষর* srcUV = বাফার + srcOffsetUV;

    unsigned অক্ষর* dst0 = bgr.তথ্য;

    জন্য (পূর্ণসংখ্যা y = 0; y < bgr.rows; y += 2)
    {
        ধ্রুবক unsigned অক্ষর* srcY1 = srcY0 + srcStepY;
        unsigned অক্ষর *dst1 = dst0 + dstStep;

        জন্য (পূর্ণসংখ্যা x = 0; x < bgr.cols; x += 2)
        {
            দশমিক Y0 = দশমিক(srcY0[x+0]);
            দশমিক Y1 = দশমিক(srcY0[x+1]);
            দশমিক Y2 = দশমিক(srcY1[x+0]);
            দশমিক Y3 = দশমিক(srcY1[x+1]);

            দশমিক U = দশমিক(srcUV[2*(x/2)+0]) - d2;
            দশমিক V = দশমিক(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            দশমিক ruv = coeffs[4]*V;
            দশমিক guv = coeffs[3]*V + coeffs[2]*U;
            দশমিক buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

স্থির শূন্য copy_convert_bgr_to_nv12(ধ্রুবক VAImage& image, ধ্রুবক Mat& bgr, unsigned অক্ষর* বাফার)
{
    ধ্রুবক দশমিক d1 = 16.0f;
    ধ্রুবক দশমিক d2 = 128.0f;

    স্থির ধ্রুবক দশমিক coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ধ্রুবক size_t dstOffsetY = image.offsets[0];
    ধ্রুবক size_t dstOffsetUV = image.offsets[1];

    ধ্রুবক size_t dstStepY = image.pitches[0];
    ধ্রুবক size_t dstStepUV = image.pitches[1];

    ধ্রুবক size_t srcStep = bgr.step;

    ধ্রুবক unsigned অক্ষর* src0 = bgr.তথ্য;

    unsigned অক্ষর* dstY0 = বাফার + dstOffsetY;
    unsigned অক্ষর* dstUV = বাফার + dstOffsetUV;

    জন্য (পূর্ণসংখ্যা y = 0; y < bgr.rows; y += 2)
    {
        ধ্রুবক unsigned অক্ষর *src1 = src0 + srcStep;
        unsigned অক্ষর* dstY1 = dstY0 + dstStepY;

        জন্য (পূর্ণসংখ্যা x = 0; x < bgr.cols; x += 2)
        {
            দশমিক B0 = দশমিক(src0[(x+0)*NCHANNELS+0]);
            দশমিক G0 = দশমিক(src0[(x+0)*NCHANNELS+1]);
            দশমিক R0 = দশমিক(src0[(x+0)*NCHANNELS+2]);

            দশমিক B1 = দশমিক(src0[(x+1)*NCHANNELS+0]);
            দশমিক G1 = দশমিক(src0[(x+1)*NCHANNELS+1]);
            দশমিক R1 = দশমিক(src0[(x+1)*NCHANNELS+2]);

            দশমিক B2 = দশমিক(src1[(x+0)*NCHANNELS+0]);
            দশমিক G2 = দশমিক(src1[(x+0)*NCHANNELS+1]);
            দশমিক R2 = দশমিক(src1[(x+0)*NCHANNELS+2]);

            দশমিক B3 = দশমিক(src1[(x+1)*NCHANNELS+0]);
            দশমিক G3 = দশমিক(src1[(x+1)*NCHANNELS+1]);
            দশমিক R3 = দশমিক(src1[(x+1)*NCHANNELS+2]);

            দশমিক Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            দশমিক Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            দশমিক Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            দশমিক Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            দশমিক U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            দশমিক V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned অক্ষর>(Y0);
            dstY0[x+1] = saturate_cast<unsigned অক্ষর>(Y1);
            dstY1[x+0] = saturate_cast<unsigned অক্ষর>(Y2);
            dstY1[x+1] = saturate_cast<unsigned অক্ষর>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned অক্ষর>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned অক্ষর>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


স্থির শূন্য copy_convert_yv12_to_bgr(ধ্রুবক VAImage& image, ধ্রুবক unsigned অক্ষর* বাফার, Mat& bgr)
{
    ধ্রুবক দশমিক d1 = 16.0f;
    ধ্রুবক দশমিক d2 = 128.0f;

    স্থির ধ্রুবক দশমিক coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.অঙ্ক_planes, (size_t)3, "");

    ধ্রুবক size_t srcOffsetY = image.offsets[0];
    ধ্রুবক size_t srcOffsetV = image.offsets[1];
    ধ্রুবক size_t srcOffsetU = image.offsets[2];

    ধ্রুবক size_t srcStepY = image.pitches[0];
    ধ্রুবক size_t srcStepU = image.pitches[1];
    ধ্রুবক size_t srcStepV = image.pitches[2];

    ধ্রুবক size_t dstStep = bgr.step;

    ধ্রুবক unsigned অক্ষর* srcY_ = বাফার + srcOffsetY;
    ধ্রুবক unsigned অক্ষর* srcV_ = বাফার + srcOffsetV;
    ধ্রুবক unsigned অক্ষর* srcU_ = বাফার + srcOffsetU;

    জন্য (পূর্ণসংখ্যা y = 0; y < bgr.rows; y += 2)
    {
        ধ্রুবক unsigned অক্ষর* srcY0 = srcY_ + (srcStepY) * y;
        ধ্রুবক unsigned অক্ষর* srcY1 = srcY0 + srcStepY;

        ধ্রুবক unsigned অক্ষর* srcV = srcV_ + (srcStepV) * y / 2;
        ধ্রুবক unsigned অক্ষর* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned অক্ষর* dst0 = bgr.তথ্য + (dstStep) * y;
        unsigned অক্ষর* dst1 = dst0 + dstStep;

        জন্য (পূর্ণসংখ্যা x = 0; x < bgr.cols; x += 2)
        {
            দশমিক Y0 = দশমিক(srcY0[x+0]);
            দশমিক Y1 = দশমিক(srcY0[x+1]);
            দশমিক Y2 = দশমিক(srcY1[x+0]);
            দশমিক Y3 = দশমিক(srcY1[x+1]);

            দশমিক U = দশমিক(srcU[x/2]) - d2;
            দশমিক V = দশমিক(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            দশমিক ruv = coeffs[4]*V;
            দশমিক guv = coeffs[3]*V + coeffs[2]*U;
            দশমিক buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned অক্ষর>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned অক্ষর>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned অক্ষর>(Y3 + ruv);
        }
    }
}

স্থির শূন্য copy_convert_bgr_to_yv12(ধ্রুবক VAImage& image, ধ্রুবক Mat& bgr, unsigned অক্ষর* বাফার)
{
    ধ্রুবক দশমিক d1 = 16.0f;
    ধ্রুবক দশমিক d2 = 128.0f;

    স্থির ধ্রুবক দশমিক coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.অঙ্ক_planes, (size_t)3, "");

    ধ্রুবক size_t dstOffsetY = image.offsets[0];
    ধ্রুবক size_t dstOffsetV = image.offsets[1];
    ধ্রুবক size_t dstOffsetU = image.offsets[2];

    ধ্রুবক size_t dstStepY = image.pitches[0];
    ধ্রুবক size_t dstStepU = image.pitches[1];
    ধ্রুবক size_t dstStepV = image.pitches[2];

    unsigned অক্ষর* dstY_ = বাফার + dstOffsetY;
    unsigned অক্ষর* dstV_ = বাফার + dstOffsetV;
    unsigned অক্ষর* dstU_ = বাফার + dstOffsetU;

    ধ্রুবক size_t srcStep = bgr.step;

    জন্য (পূর্ণসংখ্যা y = 0; y < bgr.rows; y += 2)
    {
        unsigned অক্ষর* dstY0 = dstY_ + (dstStepY) * y;
        unsigned অক্ষর* dstY1 = dstY0 + dstStepY;

        unsigned অক্ষর* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned অক্ষর* dstU = dstU_ + (dstStepU) * y / 2;

        ধ্রুবক unsigned অক্ষর* src0 = bgr.তথ্য + (srcStep) * y;
        ধ্রুবক unsigned অক্ষর* src1 = src0 + srcStep;

        জন্য (পূর্ণসংখ্যা x = 0; x < bgr.cols; x += 2)
        {
            দশমিক B0 = দশমিক(src0[(x+0)*NCHANNELS+0]);
            দশমিক G0 = দশমিক(src0[(x+0)*NCHANNELS+1]);
            দশমিক R0 = দশমিক(src0[(x+0)*NCHANNELS+2]);

            দশমিক B1 = দশমিক(src0[(x+1)*NCHANNELS+0]);
            দশমিক G1 = দশমিক(src0[(x+1)*NCHANNELS+1]);
            দশমিক R1 = দশমিক(src0[(x+1)*NCHANNELS+2]);

            দশমিক B2 = দশমিক(src1[(x+0)*NCHANNELS+0]);
            দশমিক G2 = দশমিক(src1[(x+0)*NCHANNELS+1]);
            দশমিক R2 = দশমিক(src1[(x+0)*NCHANNELS+2]);

            দশমিক B3 = দশমিক(src1[(x+1)*NCHANNELS+0]);
            দশমিক G3 = দশমিক(src1[(x+1)*NCHANNELS+1]);
            দশমিক R3 = দশমিক(src1[(x+1)*NCHANNELS+2]);

            দশমিক Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            দশমিক Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            দশমিক Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            দশমিক Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            দশমিক U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            দশমিক V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned অক্ষর>(Y0);
            dstY0[x+1] = saturate_cast<unsigned অক্ষর>(Y1);
            dstY1[x+0] = saturate_cast<unsigned অক্ষর>(Y2);
            dstY1[x+1] = saturate_cast<unsigned অক্ষর>(Y3);

            dstU[x/2] = saturate_cast<unsigned অক্ষর>(U);
            dstV[x/2] = saturate_cast<unsigned অক্ষর>(V);
        }
    }
}
#endif // HAVE_VA

শূন্য convertToVASurface(VADisplay display, প্রবেশ_Array src, VASurfaceID surface, আকার_2 size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#যদি !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#অন্যথা  // !HAVE_VA

    ধ্রুবক পূর্ণসংখ্যা stype = CV_8UC3;

    পূর্ণসংখ্যা srcType = src.type();
    CV_Assert(srcType == stype);

    আকার_2 src_আকার = src.size();
    CV_Assert(src_আকার.width == size.width && src_আকার.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ফাঁকা, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    শূন্য* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ফাঁকা, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    যৌক্তিক isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ফাঁকা, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (শূন্য*)display << "(surface)");
    যদি (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_বাফার = (cl_mem)u.handle(ACCESS_READ);

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
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ফাঁকা, ফাঁকা);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        যদি (!ocl::ocl_convert_bgr_to_nv12(cl_বাফার, (পূর্ণসংখ্যা)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ফাঁকা, ফাঁকা);
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
        CV_Assert(m.তথ্য == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        যৌক্তিক indirect_বাফার = মিথ্যা;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        যদি (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_বাফার = সত্য;
            পূর্ণসংখ্যা অঙ্ক_formats = va_Max_অঙ্ক_Image_Formats(display);
            যদি (অঙ্ক_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(অঙ্ক_formats);

            status = vaQueryImageFormats(display, fmt_list.তথ্য(), &অঙ্ক_formats);
            যদি (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            জন্য (auto &fmt : fmt_list){
                যদি (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    বিরতি;
                }
            }
            যদি (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            যদি (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned অক্ষর* বাফার = 0;
        status = va_Map_বাফার(display, image.buf, (শূন্য **)&বাফার);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        যদি (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, বাফার);
        যদি (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, বাফার);
        অন্যথা
            CV_Check((পূর্ণসংখ্যা)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = va_Unmap_বাফার(display, image.buf);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        যদি (indirect_বাফার){
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

শূন্য convertFromVASurface(VADisplay display, VASurfaceID surface, আকার_2 size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#যদি !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#অন্যথা  // !HAVE_VA

    ধ্রুবক পূর্ণসংখ্যা dtype = CV_8UC3;

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

        cl_mem cl_বাফার = (cl_mem)u.handle(ACCESS_WRITE);

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
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ফাঁকা, ফাঁকা);
        যদি (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        যদি (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, cl_বাফার, (পূর্ণসংখ্যা)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ফাঁকা, ফাঁকা);
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
        CV_Assert(m.তথ্য == m.datastart);
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
            পূর্ণসংখ্যা অঙ্ক_formats = va_Max_অঙ্ক_Image_Formats(display);
            যদি (অঙ্ক_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(অঙ্ক_formats);

            status = vaQueryImageFormats(display, fmt_list.তথ্য(), &অঙ্ক_formats);
            যদি (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            জন্য (auto &fmt : fmt_list){
                যদি (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    বিরতি;
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

        unsigned অক্ষর* বাফার = 0;
        status = va_Map_বাফার(display, image.buf, (শূন্য **)&বাফার);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        যদি (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, বাফার, m);
        যদি (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, বাফার, m);
        অন্যথা
            CV_Check((পূর্ণসংখ্যা)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = va_Unmap_বাফার(display, image.buf);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        যদি (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
