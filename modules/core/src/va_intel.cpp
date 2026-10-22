// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#وگرنه  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using فضاینام cv;

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
#وگرنه
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#شامل "va_wrapper.impl.hpp"
#وگرنه
فضاینام cv { فضاینام detail {
ایستا تهی init_libva() { /* nothing */ }
}}  // namespace
#endif
using فضاینام cv::detail;
#endif

فضاینام cv { فضاینام va_intel {

#ifdef HAVE_VA_INTEL

رده VAAPIInterop : عمومی ocl::Context::UserContext
{
عمومی:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        اگر (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    مجازی ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

فضاینام ocl {

Context& initializeContextFromVA(VADisplay display, منطقی tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#اگر !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وگرنه  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    اگر (tryInterop)
    {
        cl_uint رقم_Platforms;
        cl_int status = clGetPlatformIDs(0, پوچ, &رقم_Platforms);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        اگر (رقم_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(رقم_Platforms);
        status = clGetPlatformIDs(رقم_Platforms, &platforms[0], پوچ);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        صحیح found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        برای (صحیح i = 0; i < (صحیح)رقم_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            اگر ((تهی*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == پوچ)
            {
                ادامه;
            }

            // Query device list

            cl_uint رقم_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, پوچ, &رقم_Devices);
            اگر ((status != CL_SUCCESS) || !(رقم_Devices > 0))
                ادامه;
            رقم_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, رقم_Devices, &device, پوچ);
            اگر (status != CL_SUCCESS)
                ادامه;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, رقم_Devices, &device, پوچ, پوچ, &status);
            اگر (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            وگرنه
            {
                found = i;
                بشکن;
            }
        }

        اگر (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            تلاش
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            بگیر (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                پرتابکن;
            }
            clExecCtx.bind();
            بازگشت const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        بازگشت ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
ایستا منطقی ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, صحیح step, صحیح cols, صحیح rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    اگر (k.empty())
        بازگشت نادرست;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    بازگشت k.run(2, globalsize, 0, نادرست);
}

ایستا منطقی ocl_convert_bgr_to_nv12(cl_mem clBuffer, صحیح step, صحیح cols, صحیح rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    اگر (k.empty())
        بازگشت نادرست;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    بازگشت k.run(2, globalsize, 0, نادرست);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#اگر defined(HAVE_VA)
ثابت صحیح NCHANNELS = 3;

ایستا تهی copy_convert_nv12_to_bgr(ثابت VAImage& image, ثابت unsigned نویسه* buffer, Mat& bgr)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ایستا ثابت اعشاری coeffs[5] =
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

    ثابت unsigned نویسه* srcY0 = buffer + srcOffsetY;
    ثابت unsigned نویسه* srcUV = buffer + srcOffsetUV;

    unsigned نویسه* dst0 = bgr.data;

    برای (صحیح y = 0; y < bgr.rows; y += 2)
    {
        ثابت unsigned نویسه* srcY1 = srcY0 + srcStepY;
        unsigned نویسه *dst1 = dst0 + dstStep;

        برای (صحیح x = 0; x < bgr.cols; x += 2)
        {
            اعشاری Y0 = اعشاری(srcY0[x+0]);
            اعشاری Y1 = اعشاری(srcY0[x+1]);
            اعشاری Y2 = اعشاری(srcY1[x+0]);
            اعشاری Y3 = اعشاری(srcY1[x+1]);

            اعشاری U = اعشاری(srcUV[2*(x/2)+0]) - d2;
            اعشاری V = اعشاری(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            اعشاری ruv = coeffs[4]*V;
            اعشاری guv = coeffs[3]*V + coeffs[2]*U;
            اعشاری buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

ایستا تهی copy_convert_bgr_to_nv12(ثابت VAImage& image, ثابت Mat& bgr, unsigned نویسه* buffer)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ایستا ثابت اعشاری coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ثابت size_t dstOffsetY = image.offsets[0];
    ثابت size_t dstOffsetUV = image.offsets[1];

    ثابت size_t dstStepY = image.pitches[0];
    ثابت size_t dstStepUV = image.pitches[1];

    ثابت size_t srcStep = bgr.step;

    ثابت unsigned نویسه* src0 = bgr.data;

    unsigned نویسه* dstY0 = buffer + dstOffsetY;
    unsigned نویسه* dstUV = buffer + dstOffsetUV;

    برای (صحیح y = 0; y < bgr.rows; y += 2)
    {
        ثابت unsigned نویسه *src1 = src0 + srcStep;
        unsigned نویسه* dstY1 = dstY0 + dstStepY;

        برای (صحیح x = 0; x < bgr.cols; x += 2)
        {
            اعشاری B0 = اعشاری(src0[(x+0)*NCHANNELS+0]);
            اعشاری G0 = اعشاری(src0[(x+0)*NCHANNELS+1]);
            اعشاری R0 = اعشاری(src0[(x+0)*NCHANNELS+2]);

            اعشاری B1 = اعشاری(src0[(x+1)*NCHANNELS+0]);
            اعشاری G1 = اعشاری(src0[(x+1)*NCHANNELS+1]);
            اعشاری R1 = اعشاری(src0[(x+1)*NCHANNELS+2]);

            اعشاری B2 = اعشاری(src1[(x+0)*NCHANNELS+0]);
            اعشاری G2 = اعشاری(src1[(x+0)*NCHANNELS+1]);
            اعشاری R2 = اعشاری(src1[(x+0)*NCHANNELS+2]);

            اعشاری B3 = اعشاری(src1[(x+1)*NCHANNELS+0]);
            اعشاری G3 = اعشاری(src1[(x+1)*NCHANNELS+1]);
            اعشاری R3 = اعشاری(src1[(x+1)*NCHANNELS+2]);

            اعشاری Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            اعشاری Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            اعشاری Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            اعشاری Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            اعشاری U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            اعشاری V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned نویسه>(Y0);
            dstY0[x+1] = saturate_cast<unsigned نویسه>(Y1);
            dstY1[x+0] = saturate_cast<unsigned نویسه>(Y2);
            dstY1[x+1] = saturate_cast<unsigned نویسه>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned نویسه>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned نویسه>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


ایستا تهی copy_convert_yv12_to_bgr(ثابت VAImage& image, ثابت unsigned نویسه* buffer, Mat& bgr)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ایستا ثابت اعشاری coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.رقم_planes, (size_t)3, "");

    ثابت size_t srcOffsetY = image.offsets[0];
    ثابت size_t srcOffsetV = image.offsets[1];
    ثابت size_t srcOffsetU = image.offsets[2];

    ثابت size_t srcStepY = image.pitches[0];
    ثابت size_t srcStepU = image.pitches[1];
    ثابت size_t srcStepV = image.pitches[2];

    ثابت size_t dstStep = bgr.step;

    ثابت unsigned نویسه* srcY_ = buffer + srcOffsetY;
    ثابت unsigned نویسه* srcV_ = buffer + srcOffsetV;
    ثابت unsigned نویسه* srcU_ = buffer + srcOffsetU;

    برای (صحیح y = 0; y < bgr.rows; y += 2)
    {
        ثابت unsigned نویسه* srcY0 = srcY_ + (srcStepY) * y;
        ثابت unsigned نویسه* srcY1 = srcY0 + srcStepY;

        ثابت unsigned نویسه* srcV = srcV_ + (srcStepV) * y / 2;
        ثابت unsigned نویسه* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned نویسه* dst0 = bgr.data + (dstStep) * y;
        unsigned نویسه* dst1 = dst0 + dstStep;

        برای (صحیح x = 0; x < bgr.cols; x += 2)
        {
            اعشاری Y0 = اعشاری(srcY0[x+0]);
            اعشاری Y1 = اعشاری(srcY0[x+1]);
            اعشاری Y2 = اعشاری(srcY1[x+0]);
            اعشاری Y3 = اعشاری(srcY1[x+1]);

            اعشاری U = اعشاری(srcU[x/2]) - d2;
            اعشاری V = اعشاری(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            اعشاری ruv = coeffs[4]*V;
            اعشاری guv = coeffs[3]*V + coeffs[2]*U;
            اعشاری buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned نویسه>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned نویسه>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned نویسه>(Y3 + ruv);
        }
    }
}

ایستا تهی copy_convert_bgr_to_yv12(ثابت VAImage& image, ثابت Mat& bgr, unsigned نویسه* buffer)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ایستا ثابت اعشاری coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.رقم_planes, (size_t)3, "");

    ثابت size_t dstOffsetY = image.offsets[0];
    ثابت size_t dstOffsetV = image.offsets[1];
    ثابت size_t dstOffsetU = image.offsets[2];

    ثابت size_t dstStepY = image.pitches[0];
    ثابت size_t dstStepU = image.pitches[1];
    ثابت size_t dstStepV = image.pitches[2];

    unsigned نویسه* dstY_ = buffer + dstOffsetY;
    unsigned نویسه* dstV_ = buffer + dstOffsetV;
    unsigned نویسه* dstU_ = buffer + dstOffsetU;

    ثابت size_t srcStep = bgr.step;

    برای (صحیح y = 0; y < bgr.rows; y += 2)
    {
        unsigned نویسه* dstY0 = dstY_ + (dstStepY) * y;
        unsigned نویسه* dstY1 = dstY0 + dstStepY;

        unsigned نویسه* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned نویسه* dstU = dstU_ + (dstStepU) * y / 2;

        ثابت unsigned نویسه* src0 = bgr.data + (srcStep) * y;
        ثابت unsigned نویسه* src1 = src0 + srcStep;

        برای (صحیح x = 0; x < bgr.cols; x += 2)
        {
            اعشاری B0 = اعشاری(src0[(x+0)*NCHANNELS+0]);
            اعشاری G0 = اعشاری(src0[(x+0)*NCHANNELS+1]);
            اعشاری R0 = اعشاری(src0[(x+0)*NCHANNELS+2]);

            اعشاری B1 = اعشاری(src0[(x+1)*NCHANNELS+0]);
            اعشاری G1 = اعشاری(src0[(x+1)*NCHANNELS+1]);
            اعشاری R1 = اعشاری(src0[(x+1)*NCHANNELS+2]);

            اعشاری B2 = اعشاری(src1[(x+0)*NCHANNELS+0]);
            اعشاری G2 = اعشاری(src1[(x+0)*NCHANNELS+1]);
            اعشاری R2 = اعشاری(src1[(x+0)*NCHANNELS+2]);

            اعشاری B3 = اعشاری(src1[(x+1)*NCHANNELS+0]);
            اعشاری G3 = اعشاری(src1[(x+1)*NCHANNELS+1]);
            اعشاری R3 = اعشاری(src1[(x+1)*NCHANNELS+2]);

            اعشاری Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            اعشاری Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            اعشاری Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            اعشاری Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            اعشاری U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            اعشاری V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned نویسه>(Y0);
            dstY0[x+1] = saturate_cast<unsigned نویسه>(Y1);
            dstY1[x+0] = saturate_cast<unsigned نویسه>(Y2);
            dstY1[x+1] = saturate_cast<unsigned نویسه>(Y3);

            dstU[x/2] = saturate_cast<unsigned نویسه>(U);
            dstV[x/2] = saturate_cast<unsigned نویسه>(V);
        }
    }
}
#endif // HAVE_VA

تهی convertToVASurface(VADisplay display, درون_داد_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#اگر !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وگرنه  // !HAVE_VA

    ثابت صحیح stype = CV_8UC3;

    صحیح srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(پوچ, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    تهی* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(پوچ, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    منطقی isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(پوچ, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (تهی*)display << "(surface)");
    اگر (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, پوچ, پوچ);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        اگر (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (صحیح)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, پوچ, پوچ);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وگرنه
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        منطقی indirect_buffer = نادرست;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        اگر (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = درست;
            صحیح رقم_formats = va_Max_رقم_Image_Formats(display);
            اگر (رقم_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(رقم_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &رقم_formats);
            اگر (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            برای (auto &fmt : fmt_list){
                اگر (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    بشکن;
                }
            }
            اگر (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            اگر (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned نویسه* buffer = 0;
        status = vaMapBuffer(display, image.buf, (تهی **)&buffer);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        اگر (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        اگر (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        وگرنه
            CV_Check((صحیح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        اگر (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            اگر (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

تهی convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#اگر !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وگرنه  // !HAVE_VA

    ثابت صحیح dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    اگر (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, پوچ, پوچ);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        اگر (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (صحیح)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, پوچ, پوچ);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        اگر (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وگرنه
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        اگر (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            صحیح رقم_formats = va_Max_رقم_Image_Formats(display);
            اگر (رقم_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(رقم_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &رقم_formats);
            اگر (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            برای (auto &fmt : fmt_list){
                اگر (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    بشکن;
                }
            }
            اگر (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            اگر (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            اگر (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned نویسه* buffer = 0;
        status = vaMapBuffer(display, image.buf, (تهی **)&buffer);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        اگر (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        اگر (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        وگرنه
            CV_Check((صحیح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        اگر (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
