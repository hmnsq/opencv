// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#अन्यथा  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using नामाकाश cv;

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
#अन्यथा
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#अन्तर्भाव "va_wrapper.impl.hpp"
#अन्यथा
नामाकाश cv { नामाकाश detail {
स्थिर शून्य init_libva() { /* nothing */ }
}}  // namespace
#endif
using नामाकाश cv::detail;
#endif

नामाकाश cv { नामाकाश va_intel {

#ifdef HAVE_VA_INTEL

वर्ग VAAPIInterop : सार्वजनिक ocl::Context::UserContext
{
सार्वजनिक:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        यदि (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    आभास ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

नामाकाश ocl {

Context& initializeContextFromVA(VADisplay display, तार्किक tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#यदि !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    यदि (tryInterop)
    {
        cl_uint अंक_Platforms;
        cl_int status = clGetPlatformIDs(0, रिक्त, &अंक_Platforms);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        यदि (अंक_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(अंक_Platforms);
        status = clGetPlatformIDs(अंक_Platforms, &platforms[0], रिक्त);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        पूर्णाङ्क found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)अंक_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            यदि ((शून्य*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == रिक्त)
            {
                अनुवर्त;
            }

            // Query device list

            cl_uint अंक_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, रिक्त, &अंक_Devices);
            यदि ((status != CL_SUCCESS) || !(अंक_Devices > 0))
                अनुवर्त;
            अंक_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, अंक_Devices, &device, रिक्त);
            यदि (status != CL_SUCCESS)
                अनुवर्त;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, अंक_Devices, &device, रिक्त, रिक्त, &status);
            यदि (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            अन्यथा
            {
                found = i;
                भङ्ग;
            }
        }

        यदि (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            यत्न
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            गृहाण (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                क्षिप;
            }
            clExecCtx.bind();
            प्रत्यागम const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        प्रत्यागम ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
स्थिर तार्किक ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, पूर्णाङ्क step, पूर्णाङ्क cols, पूर्णाङ्क rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    माप_प्रकार globalsize[] = { (माप_प्रकार)cols/2, (माप_प्रकार)rows/2 };
    प्रत्यागम k.run(2, globalsize, 0, असत्य);
}

स्थिर तार्किक ocl_convert_bgr_to_nv12(cl_mem clBuffer, पूर्णाङ्क step, पूर्णाङ्क cols, पूर्णाङ्क rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    माप_प्रकार globalsize[] = { (माप_प्रकार)cols/2, (माप_प्रकार)rows/2 };
    प्रत्यागम k.run(2, globalsize, 0, असत्य);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#यदि defined(HAVE_VA)
ध्रुव पूर्णाङ्क NCHANNELS = 3;

स्थिर शून्य copy_convert_nv12_to_bgr(ध्रुव VAImage& image, ध्रुव अचिन्हित वर्ण* buffer, Mat& bgr)
{
    ध्रुव भिन्न d1 = 16.0f;
    ध्रुव भिन्न d2 = 128.0f;

    स्थिर ध्रुव भिन्न coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ध्रुव माप_प्रकार srcOffsetY = image.offsets[0];
    ध्रुव माप_प्रकार srcOffsetUV = image.offsets[1];

    ध्रुव माप_प्रकार srcStepY = image.pitches[0];
    ध्रुव माप_प्रकार srcStepUV = image.pitches[1];

    ध्रुव माप_प्रकार dstStep = bgr.step;

    ध्रुव अचिन्हित वर्ण* srcY0 = buffer + srcOffsetY;
    ध्रुव अचिन्हित वर्ण* srcUV = buffer + srcOffsetUV;

    अचिन्हित वर्ण* dst0 = bgr.data;

    प्रति (पूर्णाङ्क y = 0; y < bgr.rows; y += 2)
    {
        ध्रुव अचिन्हित वर्ण* srcY1 = srcY0 + srcStepY;
        अचिन्हित वर्ण *dst1 = dst0 + dstStep;

        प्रति (पूर्णाङ्क x = 0; x < bgr.cols; x += 2)
        {
            भिन्न Y0 = भिन्न(srcY0[x+0]);
            भिन्न Y1 = भिन्न(srcY0[x+1]);
            भिन्न Y2 = भिन्न(srcY1[x+0]);
            भिन्न Y3 = भिन्न(srcY1[x+1]);

            भिन्न U = भिन्न(srcUV[2*(x/2)+0]) - d2;
            भिन्न V = भिन्न(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            भिन्न ruv = coeffs[4]*V;
            भिन्न guv = coeffs[3]*V + coeffs[2]*U;
            भिन्न buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

स्थिर शून्य copy_convert_bgr_to_nv12(ध्रुव VAImage& image, ध्रुव Mat& bgr, अचिन्हित वर्ण* buffer)
{
    ध्रुव भिन्न d1 = 16.0f;
    ध्रुव भिन्न d2 = 128.0f;

    स्थिर ध्रुव भिन्न coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ध्रुव माप_प्रकार dstOffsetY = image.offsets[0];
    ध्रुव माप_प्रकार dstOffsetUV = image.offsets[1];

    ध्रुव माप_प्रकार dstStepY = image.pitches[0];
    ध्रुव माप_प्रकार dstStepUV = image.pitches[1];

    ध्रुव माप_प्रकार srcStep = bgr.step;

    ध्रुव अचिन्हित वर्ण* src0 = bgr.data;

    अचिन्हित वर्ण* dstY0 = buffer + dstOffsetY;
    अचिन्हित वर्ण* dstUV = buffer + dstOffsetUV;

    प्रति (पूर्णाङ्क y = 0; y < bgr.rows; y += 2)
    {
        ध्रुव अचिन्हित वर्ण *src1 = src0 + srcStep;
        अचिन्हित वर्ण* dstY1 = dstY0 + dstStepY;

        प्रति (पूर्णाङ्क x = 0; x < bgr.cols; x += 2)
        {
            भिन्न B0 = भिन्न(src0[(x+0)*NCHANNELS+0]);
            भिन्न G0 = भिन्न(src0[(x+0)*NCHANNELS+1]);
            भिन्न R0 = भिन्न(src0[(x+0)*NCHANNELS+2]);

            भिन्न B1 = भिन्न(src0[(x+1)*NCHANNELS+0]);
            भिन्न G1 = भिन्न(src0[(x+1)*NCHANNELS+1]);
            भिन्न R1 = भिन्न(src0[(x+1)*NCHANNELS+2]);

            भिन्न B2 = भिन्न(src1[(x+0)*NCHANNELS+0]);
            भिन्न G2 = भिन्न(src1[(x+0)*NCHANNELS+1]);
            भिन्न R2 = भिन्न(src1[(x+0)*NCHANNELS+2]);

            भिन्न B3 = भिन्न(src1[(x+1)*NCHANNELS+0]);
            भिन्न G3 = भिन्न(src1[(x+1)*NCHANNELS+1]);
            भिन्न R3 = भिन्न(src1[(x+1)*NCHANNELS+2]);

            भिन्न Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            भिन्न Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            भिन्न Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            भिन्न Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            भिन्न U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            भिन्न V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<अचिन्हित वर्ण>(Y0);
            dstY0[x+1] = saturate_cast<अचिन्हित वर्ण>(Y1);
            dstY1[x+0] = saturate_cast<अचिन्हित वर्ण>(Y2);
            dstY1[x+1] = saturate_cast<अचिन्हित वर्ण>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<अचिन्हित वर्ण>(U);
            dstUV[2*(x/2)+1] = saturate_cast<अचिन्हित वर्ण>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


स्थिर शून्य copy_convert_yv12_to_bgr(ध्रुव VAImage& image, ध्रुव अचिन्हित वर्ण* buffer, Mat& bgr)
{
    ध्रुव भिन्न d1 = 16.0f;
    ध्रुव भिन्न d2 = 128.0f;

    स्थिर ध्रुव भिन्न coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((माप_प्रकार)image.format.fourcc, (माप_प्रकार)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((माप_प्रकार)image.अंक_planes, (माप_प्रकार)3, "");

    ध्रुव माप_प्रकार srcOffsetY = image.offsets[0];
    ध्रुव माप_प्रकार srcOffsetV = image.offsets[1];
    ध्रुव माप_प्रकार srcOffsetU = image.offsets[2];

    ध्रुव माप_प्रकार srcStepY = image.pitches[0];
    ध्रुव माप_प्रकार srcStepU = image.pitches[1];
    ध्रुव माप_प्रकार srcStepV = image.pitches[2];

    ध्रुव माप_प्रकार dstStep = bgr.step;

    ध्रुव अचिन्हित वर्ण* srcY_ = buffer + srcOffsetY;
    ध्रुव अचिन्हित वर्ण* srcV_ = buffer + srcOffsetV;
    ध्रुव अचिन्हित वर्ण* srcU_ = buffer + srcOffsetU;

    प्रति (पूर्णाङ्क y = 0; y < bgr.rows; y += 2)
    {
        ध्रुव अचिन्हित वर्ण* srcY0 = srcY_ + (srcStepY) * y;
        ध्रुव अचिन्हित वर्ण* srcY1 = srcY0 + srcStepY;

        ध्रुव अचिन्हित वर्ण* srcV = srcV_ + (srcStepV) * y / 2;
        ध्रुव अचिन्हित वर्ण* srcU = srcU_ + (srcStepU) * y / 2;

        अचिन्हित वर्ण* dst0 = bgr.data + (dstStep) * y;
        अचिन्हित वर्ण* dst1 = dst0 + dstStep;

        प्रति (पूर्णाङ्क x = 0; x < bgr.cols; x += 2)
        {
            भिन्न Y0 = भिन्न(srcY0[x+0]);
            भिन्न Y1 = भिन्न(srcY0[x+1]);
            भिन्न Y2 = भिन्न(srcY1[x+0]);
            भिन्न Y3 = भिन्न(srcY1[x+1]);

            भिन्न U = भिन्न(srcU[x/2]) - d2;
            भिन्न V = भिन्न(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            भिन्न ruv = coeffs[4]*V;
            भिन्न guv = coeffs[3]*V + coeffs[2]*U;
            भिन्न buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित वर्ण>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित वर्ण>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित वर्ण>(Y3 + ruv);
        }
    }
}

स्थिर शून्य copy_convert_bgr_to_yv12(ध्रुव VAImage& image, ध्रुव Mat& bgr, अचिन्हित वर्ण* buffer)
{
    ध्रुव भिन्न d1 = 16.0f;
    ध्रुव भिन्न d2 = 128.0f;

    स्थिर ध्रुव भिन्न coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((माप_प्रकार)image.format.fourcc, (माप_प्रकार)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((माप_प्रकार)image.अंक_planes, (माप_प्रकार)3, "");

    ध्रुव माप_प्रकार dstOffsetY = image.offsets[0];
    ध्रुव माप_प्रकार dstOffsetV = image.offsets[1];
    ध्रुव माप_प्रकार dstOffsetU = image.offsets[2];

    ध्रुव माप_प्रकार dstStepY = image.pitches[0];
    ध्रुव माप_प्रकार dstStepU = image.pitches[1];
    ध्रुव माप_प्रकार dstStepV = image.pitches[2];

    अचिन्हित वर्ण* dstY_ = buffer + dstOffsetY;
    अचिन्हित वर्ण* dstV_ = buffer + dstOffsetV;
    अचिन्हित वर्ण* dstU_ = buffer + dstOffsetU;

    ध्रुव माप_प्रकार srcStep = bgr.step;

    प्रति (पूर्णाङ्क y = 0; y < bgr.rows; y += 2)
    {
        अचिन्हित वर्ण* dstY0 = dstY_ + (dstStepY) * y;
        अचिन्हित वर्ण* dstY1 = dstY0 + dstStepY;

        अचिन्हित वर्ण* dstV = dstV_ + (dstStepV) * y / 2;
        अचिन्हित वर्ण* dstU = dstU_ + (dstStepU) * y / 2;

        ध्रुव अचिन्हित वर्ण* src0 = bgr.data + (srcStep) * y;
        ध्रुव अचिन्हित वर्ण* src1 = src0 + srcStep;

        प्रति (पूर्णाङ्क x = 0; x < bgr.cols; x += 2)
        {
            भिन्न B0 = भिन्न(src0[(x+0)*NCHANNELS+0]);
            भिन्न G0 = भिन्न(src0[(x+0)*NCHANNELS+1]);
            भिन्न R0 = भिन्न(src0[(x+0)*NCHANNELS+2]);

            भिन्न B1 = भिन्न(src0[(x+1)*NCHANNELS+0]);
            भिन्न G1 = भिन्न(src0[(x+1)*NCHANNELS+1]);
            भिन्न R1 = भिन्न(src0[(x+1)*NCHANNELS+2]);

            भिन्न B2 = भिन्न(src1[(x+0)*NCHANNELS+0]);
            भिन्न G2 = भिन्न(src1[(x+0)*NCHANNELS+1]);
            भिन्न R2 = भिन्न(src1[(x+0)*NCHANNELS+2]);

            भिन्न B3 = भिन्न(src1[(x+1)*NCHANNELS+0]);
            भिन्न G3 = भिन्न(src1[(x+1)*NCHANNELS+1]);
            भिन्न R3 = भिन्न(src1[(x+1)*NCHANNELS+2]);

            भिन्न Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            भिन्न Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            भिन्न Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            भिन्न Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            भिन्न U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            भिन्न V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<अचिन्हित वर्ण>(Y0);
            dstY0[x+1] = saturate_cast<अचिन्हित वर्ण>(Y1);
            dstY1[x+0] = saturate_cast<अचिन्हित वर्ण>(Y2);
            dstY1[x+1] = saturate_cast<अचिन्हित वर्ण>(Y3);

            dstU[x/2] = saturate_cast<अचिन्हित वर्ण>(U);
            dstV[x/2] = saturate_cast<अचिन्हित वर्ण>(V);
        }
    }
}
#endif // HAVE_VA

शून्य convertToVASurface(VADisplay display, निवेश_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#यदि !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

    ध्रुव पूर्णाङ्क stype = CV_8UC3;

    पूर्णाङ्क srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(रिक्त, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    शून्य* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(रिक्त, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    तार्किक isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(रिक्त, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (शून्य*)display << "(surface)");
    यदि (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        यदि (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (पूर्णाङ्क)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    अन्यथा
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        तार्किक indirect_buffer = असत्य;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        यदि (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = सत्य;
            पूर्णाङ्क अंक_formats = va_Max_अंक_Image_Formats(display);
            यदि (अंक_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(अंक_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &अंक_formats);
            यदि (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            प्रति (स्वतः &fmt : fmt_list){
                यदि (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    भङ्ग;
                }
            }
            यदि (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            यदि (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        अचिन्हित वर्ण* buffer = 0;
        status = vaMapBuffer(display, image.buf, (शून्य **)&buffer);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        यदि (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        यदि (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        अन्यथा
            CV_Check((पूर्णाङ्क)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        यदि (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            यदि (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

शून्य convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#यदि !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

    ध्रुव पूर्णाङ्क dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    यदि (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        यदि (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (पूर्णाङ्क)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        यदि (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    अन्यथा
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        यदि (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            पूर्णाङ्क अंक_formats = va_Max_अंक_Image_Formats(display);
            यदि (अंक_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(अंक_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &अंक_formats);
            यदि (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            प्रति (स्वतः &fmt : fmt_list){
                यदि (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    भङ्ग;
                }
            }
            यदि (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            यदि (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            यदि (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        अचिन्हित वर्ण* buffer = 0;
        status = vaMapBuffer(display, image.buf, (शून्य **)&buffer);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        यदि (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        यदि (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        अन्यथा
            CV_Check((पूर्णाङ्क)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        यदि (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
