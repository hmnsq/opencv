// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#وإلا  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using نطاق cv;

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
#وإلا
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#ضمن "va_wrapper.impl.hpp"
#وإلا
نطاق cv { نطاق detail {
ساكن فراغ init_libva() { /* nothing */ }
}}  // namespace
#endif
using نطاق cv::detail;
#endif

نطاق cv { نطاق va_intel {

#ifdef HAVE_VA_INTEL

صنف VAAPIInterop : عام ocl::Context::UserContext
{
عام:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        إذا (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    virtual ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

نطاق ocl {

Context& initializeContextFromVA(VADisplay display, منطقي tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    إذا (tryInterop)
    {
        cl_uint رەقەم_Platforms;
        cl_int status = clGetPlatformIDs(0, عدم, &رەقەم_Platforms);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        إذا (رەقەم_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(رەقەم_Platforms);
        status = clGetPlatformIDs(رەقەم_Platforms, &platforms[0], عدم);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        صحيح found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        لكل (صحيح i = 0; i < (صحيح)رەقەم_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            إذا ((فراغ*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == عدم)
            {
                استمر;
            }

            // Query device list

            cl_uint رەقەم_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, عدم, &رەقەم_Devices);
            إذا ((status != CL_SUCCESS) || !(رەقەم_Devices > 0))
                استمر;
            رەقەم_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, رەقەم_Devices, &device, عدم);
            إذا (status != CL_SUCCESS)
                استمر;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, رەقەم_Devices, &device, عدم, عدم, &status);
            إذا (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            وإلا
            {
                found = i;
                اكسر;
            }
        }

        إذا (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            حاول
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            امسك (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ارم;
            }
            clExecCtx.bind();
            أرجع const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        أرجع ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
ساكن منطقي ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, صحيح step, صحيح cols, صحيح rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    أرجع k.run(2, globalsize, 0, خطأ);
}

ساكن منطقي ocl_convert_bgr_to_nv12(cl_mem clBuffer, صحيح step, صحيح cols, صحيح rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    أرجع k.run(2, globalsize, 0, خطأ);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#إذا defined(HAVE_VA)
ثابت صحيح NCHANNELS = 3;

ساكن فراغ copy_convert_nv12_to_bgr(ثابت VAImage& image, ثابت unsigned حرف* buffer, Mat& bgr)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ثابت size_t srcOffsetY = image.offsets[0];
    ثابت size_t srcOffsetUV = image.offsets[1];

    ثابت size_t srcStepY = image.pitches[0];
    ثابت size_t srcStepUV = image.pitches[1];

    ثابت size_t dstStep = bgr.step;

    ثابت unsigned حرف* srcY0 = buffer + srcOffsetY;
    ثابت unsigned حرف* srcUV = buffer + srcOffsetUV;

    unsigned حرف* dst0 = bgr.data;

    لكل (صحيح y = 0; y < bgr.rows; y += 2)
    {
        ثابت unsigned حرف* srcY1 = srcY0 + srcStepY;
        unsigned حرف *dst1 = dst0 + dstStep;

        لكل (صحيح x = 0; x < bgr.cols; x += 2)
        {
            عائم Y0 = عائم(srcY0[x+0]);
            عائم Y1 = عائم(srcY0[x+1]);
            عائم Y2 = عائم(srcY1[x+0]);
            عائم Y3 = عائم(srcY1[x+1]);

            عائم U = عائم(srcUV[2*(x/2)+0]) - d2;
            عائم V = عائم(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            عائم ruv = coeffs[4]*V;
            عائم guv = coeffs[3]*V + coeffs[2]*U;
            عائم buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

ساكن فراغ copy_convert_bgr_to_nv12(ثابت VAImage& image, ثابت Mat& bgr, unsigned حرف* buffer)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ثابت size_t dstOffsetY = image.offsets[0];
    ثابت size_t dstOffsetUV = image.offsets[1];

    ثابت size_t dstStepY = image.pitches[0];
    ثابت size_t dstStepUV = image.pitches[1];

    ثابت size_t srcStep = bgr.step;

    ثابت unsigned حرف* src0 = bgr.data;

    unsigned حرف* dstY0 = buffer + dstOffsetY;
    unsigned حرف* dstUV = buffer + dstOffsetUV;

    لكل (صحيح y = 0; y < bgr.rows; y += 2)
    {
        ثابت unsigned حرف *src1 = src0 + srcStep;
        unsigned حرف* dstY1 = dstY0 + dstStepY;

        لكل (صحيح x = 0; x < bgr.cols; x += 2)
        {
            عائم B0 = عائم(src0[(x+0)*NCHANNELS+0]);
            عائم G0 = عائم(src0[(x+0)*NCHANNELS+1]);
            عائم R0 = عائم(src0[(x+0)*NCHANNELS+2]);

            عائم B1 = عائم(src0[(x+1)*NCHANNELS+0]);
            عائم G1 = عائم(src0[(x+1)*NCHANNELS+1]);
            عائم R1 = عائم(src0[(x+1)*NCHANNELS+2]);

            عائم B2 = عائم(src1[(x+0)*NCHANNELS+0]);
            عائم G2 = عائم(src1[(x+0)*NCHANNELS+1]);
            عائم R2 = عائم(src1[(x+0)*NCHANNELS+2]);

            عائم B3 = عائم(src1[(x+1)*NCHANNELS+0]);
            عائم G3 = عائم(src1[(x+1)*NCHANNELS+1]);
            عائم R3 = عائم(src1[(x+1)*NCHANNELS+2]);

            عائم Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            عائم Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            عائم Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            عائم Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            عائم U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            عائم V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned حرف>(Y0);
            dstY0[x+1] = saturate_cast<unsigned حرف>(Y1);
            dstY1[x+0] = saturate_cast<unsigned حرف>(Y2);
            dstY1[x+1] = saturate_cast<unsigned حرف>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned حرف>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned حرف>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


ساكن فراغ copy_convert_yv12_to_bgr(ثابت VAImage& image, ثابت unsigned حرف* buffer, Mat& bgr)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.رەقەم_planes, (size_t)3, "");

    ثابت size_t srcOffsetY = image.offsets[0];
    ثابت size_t srcOffsetV = image.offsets[1];
    ثابت size_t srcOffsetU = image.offsets[2];

    ثابت size_t srcStepY = image.pitches[0];
    ثابت size_t srcStepU = image.pitches[1];
    ثابت size_t srcStepV = image.pitches[2];

    ثابت size_t dstStep = bgr.step;

    ثابت unsigned حرف* srcY_ = buffer + srcOffsetY;
    ثابت unsigned حرف* srcV_ = buffer + srcOffsetV;
    ثابت unsigned حرف* srcU_ = buffer + srcOffsetU;

    لكل (صحيح y = 0; y < bgr.rows; y += 2)
    {
        ثابت unsigned حرف* srcY0 = srcY_ + (srcStepY) * y;
        ثابت unsigned حرف* srcY1 = srcY0 + srcStepY;

        ثابت unsigned حرف* srcV = srcV_ + (srcStepV) * y / 2;
        ثابت unsigned حرف* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned حرف* dst0 = bgr.data + (dstStep) * y;
        unsigned حرف* dst1 = dst0 + dstStep;

        لكل (صحيح x = 0; x < bgr.cols; x += 2)
        {
            عائم Y0 = عائم(srcY0[x+0]);
            عائم Y1 = عائم(srcY0[x+1]);
            عائم Y2 = عائم(srcY1[x+0]);
            عائم Y3 = عائم(srcY1[x+1]);

            عائم U = عائم(srcU[x/2]) - d2;
            عائم V = عائم(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            عائم ruv = coeffs[4]*V;
            عائم guv = coeffs[3]*V + coeffs[2]*U;
            عائم buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned حرف>(Y3 + ruv);
        }
    }
}

ساكن فراغ copy_convert_bgr_to_yv12(ثابت VAImage& image, ثابت Mat& bgr, unsigned حرف* buffer)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.رەقەم_planes, (size_t)3, "");

    ثابت size_t dstOffsetY = image.offsets[0];
    ثابت size_t dstOffsetV = image.offsets[1];
    ثابت size_t dstOffsetU = image.offsets[2];

    ثابت size_t dstStepY = image.pitches[0];
    ثابت size_t dstStepU = image.pitches[1];
    ثابت size_t dstStepV = image.pitches[2];

    unsigned حرف* dstY_ = buffer + dstOffsetY;
    unsigned حرف* dstV_ = buffer + dstOffsetV;
    unsigned حرف* dstU_ = buffer + dstOffsetU;

    ثابت size_t srcStep = bgr.step;

    لكل (صحيح y = 0; y < bgr.rows; y += 2)
    {
        unsigned حرف* dstY0 = dstY_ + (dstStepY) * y;
        unsigned حرف* dstY1 = dstY0 + dstStepY;

        unsigned حرف* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned حرف* dstU = dstU_ + (dstStepU) * y / 2;

        ثابت unsigned حرف* src0 = bgr.data + (srcStep) * y;
        ثابت unsigned حرف* src1 = src0 + srcStep;

        لكل (صحيح x = 0; x < bgr.cols; x += 2)
        {
            عائم B0 = عائم(src0[(x+0)*NCHANNELS+0]);
            عائم G0 = عائم(src0[(x+0)*NCHANNELS+1]);
            عائم R0 = عائم(src0[(x+0)*NCHANNELS+2]);

            عائم B1 = عائم(src0[(x+1)*NCHANNELS+0]);
            عائم G1 = عائم(src0[(x+1)*NCHANNELS+1]);
            عائم R1 = عائم(src0[(x+1)*NCHANNELS+2]);

            عائم B2 = عائم(src1[(x+0)*NCHANNELS+0]);
            عائم G2 = عائم(src1[(x+0)*NCHANNELS+1]);
            عائم R2 = عائم(src1[(x+0)*NCHANNELS+2]);

            عائم B3 = عائم(src1[(x+1)*NCHANNELS+0]);
            عائم G3 = عائم(src1[(x+1)*NCHANNELS+1]);
            عائم R3 = عائم(src1[(x+1)*NCHANNELS+2]);

            عائم Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            عائم Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            عائم Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            عائم Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            عائم U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            عائم V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned حرف>(Y0);
            dstY0[x+1] = saturate_cast<unsigned حرف>(Y1);
            dstY1[x+0] = saturate_cast<unsigned حرف>(Y2);
            dstY1[x+1] = saturate_cast<unsigned حرف>(Y3);

            dstU[x/2] = saturate_cast<unsigned حرف>(U);
            dstV[x/2] = saturate_cast<unsigned حرف>(V);
        }
    }
}
#endif // HAVE_VA

فراغ convertToVASurface(VADisplay display, كىرگۈزۈش_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

    ثابت صحيح stype = CV_8UC3;

    صحيح srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(عدم, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    فراغ* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(عدم, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    منطقي isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(عدم, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (فراغ*)display << "(surface)");
    إذا (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, عدم, عدم);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        إذا (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (صحيح)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, عدم, عدم);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وإلا
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        منطقي indirect_buffer = خطأ;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        إذا (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = true;
            صحيح رەقەم_formats = va_Max_رەقەم_Image_Formats(display);
            إذا (رەقەم_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(رەقەم_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &رەقەم_formats);
            إذا (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            لكل (auto &fmt : fmt_list){
                إذا (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    اكسر;
                }
            }
            إذا (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            إذا (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned حرف* buffer = 0;
        status = vaMapBuffer(display, image.buf, (فراغ **)&buffer);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        إذا (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        إذا (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        وإلا
            CV_Check((صحيح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        إذا (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            إذا (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

فراغ convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

    ثابت صحيح dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    إذا (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, عدم, عدم);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        إذا (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (صحيح)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, عدم, عدم);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        إذا (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وإلا
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        إذا (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            صحيح رەقەم_formats = va_Max_رەقەم_Image_Formats(display);
            إذا (رەقەم_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(رەقەم_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &رەقەم_formats);
            إذا (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            لكل (auto &fmt : fmt_list){
                إذا (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    اكسر;
                }
            }
            إذا (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            إذا (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            إذا (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned حرف* buffer = 0;
        status = vaMapBuffer(display, image.buf, (فراغ **)&buffer);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        إذا (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        إذا (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        وإلا
            CV_Check((صحيح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        إذا (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
