// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/logger.hpp>

#اگر_تعریف_شده HAVE_VA
#  include <va/va.h>
#وإلا  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#پایان_اگر // HAVE_VA

استفاده نطاق cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#اگر_تعریف_شده HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#پایان_اگر // HAVE_OPENCL

#اگر_تعریف_شده HAVE_VA_INTEL
#اگر_تعریف_شده HAVE_VA_INTEL_OLD_سرآیند
#  include <CL/va_ext.h>
#وإلا
#  include <CL/cl_va_api_media_sharing_intel.h>
#پایان_اگر
#پایان_اگر

#اگر_تعریف_شده HAVE_VA
#اگر_تعریف_نشده OPENCV_LIBVA_LINK
#ضمن "va_wrapper.impl.hpp"
#وإلا
نطاق cv { نطاق detail {
ساكن فراغ init_libva() { /* nothing */ }
}}  // namespace
#پایان_اگر
استفاده نطاق cv::detail;
#پایان_اگر

نطاق cv { نطاق va_intel {

#اگر_تعریف_شده HAVE_VA_INTEL

صنف VAAPIInterop : عام ocl::Context::UserContext
{
عام:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                cl_Get_Extension_Function_آدرس_For_Platform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        cl_افزودن_به_صف_Acquire_VA_APIMedia_Surfaces_INTEL  = (cl_افزودن_به_صف_Acquire_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_آدرس_For_Platform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        cl_افزودن_به_صف_Release_VA_APIMedia_Surfaces_INTEL  = (cl_افزودن_به_صف_Release_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_آدرس_For_Platform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        إذا (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !cl_افزودن_به_صف_Acquire_VA_APIMedia_Surfaces_INTEL ||
            !cl_افزودن_به_صف_Release_VA_APIMedia_Surfaces_INTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    مجازی ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    cl_افزودن_به_صف_Acquire_VA_APIMedia_Surfaces_INTEL_fn  cl_افزودن_به_صف_Acquire_VA_APIMedia_Surfaces_INTEL;
    cl_افزودن_به_صف_Release_VA_APIMedia_Surfaces_INTEL_fn  cl_افزودن_به_صف_Release_VA_APIMedia_Surfaces_INTEL;
};

#پایان_اگر // HAVE_VA_INTEL

نطاق ocl {

Context& initializeContextFromVA(VADisplay display, منطقي tryInterop)
{
    CV_استفادهنشده(display); CV_استفادهنشده(tryInterop);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    إذا (tryInterop)
    {
        cl_uint رقم_Platforms;
        cl_int وضعیت = clGetPlatformIDs(0, عدم, &رقم_Platforms);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        إذا (رقم_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(رقم_Platforms);
        وضعیت = clGetPlatformIDs(رقم_Platforms, &platforms[0], عدم);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        صحيح یافت = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        لكل (صحيح i = 0; i < (صحيح)رقم_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                cl_Get_Extension_Function_آدرس_For_Platform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            إذا ((فراغ*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == عدم)
            {
                استمر;
            }

            // Query device list

            cl_uint رقم_Devices = 0;

            وضعیت = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, عدم, &رقم_Devices);
            إذا ((وضعیت != CL_SUCCESS) || !(رقم_Devices > 0))
                استمر;
            رقم_Devices = 1; // OpenCV expects only 1 device
            وضعیت = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, رقم_Devices, &device, عدم);
            إذا (وضعیت != CL_SUCCESS)
                استمر;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, رقم_Devices, &device, عدم, عدم, &وضعیت);
            إذا (وضعیت != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            وإلا
            {
                یافت = i;
                اكسر;
            }
        }

        إذا (یافت >= 0)
        {
            cl_platform_id platform = platforms[یافت];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            حاول
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().مجموعه_User_Context(std::make_shared<VAAPIInterop>(platform));
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
#پایان_اگر  // !HAVE_VA
}

#اگر_تعریف_شده HAVE_VA_INTEL
ساكن منطقي ocl_convert_nv12_مقصد_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_میانگیر, صحيح گام, صحيح تعداد_ستون, صحيح ردیفها)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.آرگومانها(clImageY, clImageUV, cl_میانگیر, گام, تعداد_ستون, ردیفها);

    size_t globalsize[] = { (size_t)تعداد_ستون/2, (size_t)ردیفها/2 };
    أرجع k.پیاپی(2, globalsize, 0, خطأ);
}

ساكن منطقي ocl_convert_bgr_مقصد_nv12(cl_mem cl_میانگیر, صحيح گام, صحيح تعداد_ستون, صحيح ردیفها, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.آرگومانها(cl_میانگیر, گام, تعداد_ستون, ردیفها, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)تعداد_ستون/2, (size_t)ردیفها/2 };
    أرجع k.پیاپی(2, globalsize, 0, خطأ);
}
#پایان_اگر // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#إذا defined(HAVE_VA)
ثابت صحيح NCHANNELS = 3;

ساكن فراغ copy_convert_nv12_مقصد_bgr(ثابت VAImage& image, ثابت بیعلامت حرف* میانگیر, Mat& bgr)
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

    ثابت size_t src_جابه_جایی_Y = image.offsets[0];
    ثابت size_t src_جابه_جایی_UV = image.offsets[1];

    ثابت size_t src_گام_Y = image.pitches[0];
    ثابت size_t src_گام_UV = image.pitches[1];

    ثابت size_t dst_گام_3 = bgr.گام;

    ثابت بیعلامت حرف* srcY0 = میانگیر + src_جابه_جایی_Y;
    ثابت بیعلامت حرف* srcUV = میانگیر + src_جابه_جایی_UV;

    بیعلامت حرف* dst0 = bgr.داده;

    لكل (صحيح y = 0; y < bgr.ردیفها; y += 2)
    {
        ثابت بیعلامت حرف* srcY1 = srcY0 + src_گام_Y;
        بیعلامت حرف *dst1 = dst0 + dst_گام_3;

        لكل (صحيح x = 0; x < bgr.تعداد_ستون; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y3 + ruv);
        }

        srcY0 = srcY1 + src_گام_Y;
        srcUV += src_گام_UV;
        dst0 = dst1 + dst_گام_3;
    }
}

ساكن فراغ copy_convert_bgr_مقصد_nv12(ثابت VAImage& image, ثابت Mat& bgr, بیعلامت حرف* میانگیر)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ثابت size_t dst_جابه_جایی_Y = image.offsets[0];
    ثابت size_t dst_جابه_جایی_UV = image.offsets[1];

    ثابت size_t dst_گام_Y = image.pitches[0];
    ثابت size_t dst_گام_UV = image.pitches[1];

    ثابت size_t src_گام_3 = bgr.گام;

    ثابت بیعلامت حرف* src0 = bgr.داده;

    بیعلامت حرف* dstY0 = میانگیر + dst_جابه_جایی_Y;
    بیعلامت حرف* dstUV = میانگیر + dst_جابه_جایی_UV;

    لكل (صحيح y = 0; y < bgr.ردیفها; y += 2)
    {
        ثابت بیعلامت حرف *src1 = src0 + src_گام_3;
        بیعلامت حرف* dstY1 = dstY0 + dst_گام_Y;

        لكل (صحيح x = 0; x < bgr.تعداد_ستون; x += 2)
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

            dstY0[x+0] = saturate_cast<بیعلامت حرف>(Y0);
            dstY0[x+1] = saturate_cast<بیعلامت حرف>(Y1);
            dstY1[x+0] = saturate_cast<بیعلامت حرف>(Y2);
            dstY1[x+1] = saturate_cast<بیعلامت حرف>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<بیعلامت حرف>(U);
            dstUV[2*(x/2)+1] = saturate_cast<بیعلامت حرف>(V);
        }

        src0 = src1 + src_گام_3;
        dstY0 = dstY1 + dst_گام_Y;
        dstUV += dst_گام_UV;
    }
}


ساكن فراغ copy_convert_yv12_مقصد_bgr(ثابت VAImage& image, ثابت بیعلامت حرف* میانگیر, Mat& bgr)
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
    CV_CheckEQ((size_t)image.رقم_planes, (size_t)3, "");

    ثابت size_t src_جابه_جایی_Y = image.offsets[0];
    ثابت size_t src_جابه_جایی_V = image.offsets[1];
    ثابت size_t src_جابه_جایی_U = image.offsets[2];

    ثابت size_t src_گام_Y = image.pitches[0];
    ثابت size_t src_گام_U = image.pitches[1];
    ثابت size_t src_گام_V = image.pitches[2];

    ثابت size_t dst_گام_3 = bgr.گام;

    ثابت بیعلامت حرف* srcY_ = میانگیر + src_جابه_جایی_Y;
    ثابت بیعلامت حرف* srcV_ = میانگیر + src_جابه_جایی_V;
    ثابت بیعلامت حرف* srcU_ = میانگیر + src_جابه_جایی_U;

    لكل (صحيح y = 0; y < bgr.ردیفها; y += 2)
    {
        ثابت بیعلامت حرف* srcY0 = srcY_ + (src_گام_Y) * y;
        ثابت بیعلامت حرف* srcY1 = srcY0 + src_گام_Y;

        ثابت بیعلامت حرف* srcV = srcV_ + (src_گام_V) * y / 2;
        ثابت بیعلامت حرف* srcU = srcU_ + (src_گام_U) * y / 2;

        بیعلامت حرف* dst0 = bgr.داده + (dst_گام_3) * y;
        بیعلامت حرف* dst1 = dst0 + dst_گام_3;

        لكل (صحيح x = 0; x < bgr.تعداد_ستون; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<بیعلامت حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<بیعلامت حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<بیعلامت حرف>(Y3 + ruv);
        }
    }
}

ساكن فراغ copy_convert_bgr_مقصد_yv12(ثابت VAImage& image, ثابت Mat& bgr, بیعلامت حرف* میانگیر)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.رقم_planes, (size_t)3, "");

    ثابت size_t dst_جابه_جایی_Y = image.offsets[0];
    ثابت size_t dst_جابه_جایی_V = image.offsets[1];
    ثابت size_t dst_جابه_جایی_U = image.offsets[2];

    ثابت size_t dst_گام_Y = image.pitches[0];
    ثابت size_t dst_گام_U = image.pitches[1];
    ثابت size_t dst_گام_V = image.pitches[2];

    بیعلامت حرف* dstY_ = میانگیر + dst_جابه_جایی_Y;
    بیعلامت حرف* dstV_ = میانگیر + dst_جابه_جایی_V;
    بیعلامت حرف* dstU_ = میانگیر + dst_جابه_جایی_U;

    ثابت size_t src_گام_3 = bgr.گام;

    لكل (صحيح y = 0; y < bgr.ردیفها; y += 2)
    {
        بیعلامت حرف* dstY0 = dstY_ + (dst_گام_Y) * y;
        بیعلامت حرف* dstY1 = dstY0 + dst_گام_Y;

        بیعلامت حرف* dstV = dstV_ + (dst_گام_V) * y / 2;
        بیعلامت حرف* dstU = dstU_ + (dst_گام_U) * y / 2;

        ثابت بیعلامت حرف* src0 = bgr.داده + (src_گام_3) * y;
        ثابت بیعلامت حرف* src1 = src0 + src_گام_3;

        لكل (صحيح x = 0; x < bgr.تعداد_ستون; x += 2)
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

            dstY0[x+0] = saturate_cast<بیعلامت حرف>(Y0);
            dstY0[x+1] = saturate_cast<بیعلامت حرف>(Y1);
            dstY1[x+0] = saturate_cast<بیعلامت حرف>(Y2);
            dstY1[x+1] = saturate_cast<بیعلامت حرف>(Y3);

            dstU[x/2] = saturate_cast<بیعلامت حرف>(U);
            dstV[x/2] = saturate_cast<بیعلامت حرف>(V);
        }
    }
}
#پایان_اگر // HAVE_VA

فراغ convert_مقصد_VASurface(VADisplay display, درون_داد_Array src, VASurfaceID surface, اندازه size)
{
    CV_استفادهنشده(display); CV_استفادهنشده(src); CV_استفادهنشده(surface); CV_استفادهنشده(size);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

    ثابت صحيح stype = CV_8UC3;

    صحيح srcType = src.type();
    CV_Assert(srcType == stype);

    اندازه src_اندازه = src.size();
    CV_Assert(src_اندازه.عرض == size.عرض && src_اندازه.ارتفاع == size.ارتفاع);

#اگر_تعریف_شده HAVE_VA_INTEL
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

        cl_mem cl_میانگیر = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int وضعیت = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &وضعیت);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &وضعیت);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_صف q = (cl_command_صف)ocl_context.get_صف().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        وضعیت = interop->cl_افزودن_به_صف_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        إذا (!ocl::ocl_convert_bgr_مقصد_nv12(cl_میانگیر, (صحيح)u.گام[0], u.تعداد_ستون, u.ردیفها, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->cl_افزودن_به_صف_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        وضعیت = cl_پایان(q); // TODO Use events
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        وضعیت = clReleaseMemObject(clImageY); // TODO RAII
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        وضعیت = clReleaseMemObject(clImageUV);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وإلا
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.داده == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus وضعیت = 0;

        وضعیت = vaSyncSurface(display, surface);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        منطقي غیرمستقیم_میانگیر = خطأ;
        VAImage image;
        وضعیت = vaDeriveImage(display, surface, &image);
        إذا (وضعیت != VA_وضعیت_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            غیرمستقیم_میانگیر = درست;
            صحيح رقم_formats = va_Max_رقم_Image_Formats(display);
            إذا (رقم_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(رقم_formats);

            وضعیت = vaQueryImageFormats(display, fmt_list.داده(), &رقم_formats);
            إذا (وضعیت != VA_وضعیت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = اشاره_تهی;
            لكل (خودکار &fmt : fmt_list){
                إذا (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    اكسر;
                }
            }
            إذا (selected_format == اشاره_تهی)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            وضعیت = vaCreateImage(display, selected_format, size.عرض, size.ارتفاع, &image);
            إذا (وضعیت != VA_وضعیت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        بیعلامت حرف* میانگیر = 0;
        وضعیت = va_Map_میانگیر(display, image.buf, (فراغ **)&میانگیر);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        إذا (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_مقصد_nv12(image, m, میانگیر);
        إذا (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_مقصد_yv12(image, m, میانگیر);
        وإلا
            CV_Check((صحيح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        وضعیت = va_Unmap_میانگیر(display, image.buf);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        إذا (غیرمستقیم_میانگیر){
            وضعیت = vaPutImage(display, surface, image.image_id, 0, 0, size.عرض, size.ارتفاع, 0, 0, size.عرض, size.ارتفاع);
            إذا (وضعیت != VA_وضعیت_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        وضعیت = vaDestroyImage(display, image.image_id);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#پایان_اگر  // !HAVE_VA
}

فراغ convertFromVASurface(VADisplay display, VASurfaceID surface, اندازه size, OutputArray dst)
{
    CV_استفادهنشده(display); CV_استفادهنشده(surface); CV_استفادهنشده(dst); CV_استفادهنشده(size);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

    ثابت صحيح dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#اگر_تعریف_شده HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    إذا (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_میانگیر = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int وضعیت = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &وضعیت);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &وضعیت);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_صف q = (cl_command_صف)ocl_context.get_صف().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        وضعیت = interop->cl_افزودن_به_صف_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        إذا (!ocl::ocl_convert_nv12_مقصد_bgr(clImageY, clImageUV, cl_میانگیر, (صحيح)u.گام[0], u.تعداد_ستون, u.ردیفها))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        وضعیت = interop->cl_افزودن_به_صف_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        وضعیت = cl_پایان(q); // TODO Use events
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        وضعیت = clReleaseMemObject(clImageY); // TODO RAII
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        وضعیت = clReleaseMemObject(clImageUV);
        إذا (وضعیت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وإلا
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.داده == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus وضعیت = 0;

        وضعیت = vaSyncSurface(display, surface);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        وضعیت = vaDeriveImage(display, surface, &image);
        إذا (وضعیت != VA_وضعیت_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            صحيح رقم_formats = va_Max_رقم_Image_Formats(display);
            إذا (رقم_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(رقم_formats);

            وضعیت = vaQueryImageFormats(display, fmt_list.داده(), &رقم_formats);
            إذا (وضعیت != VA_وضعیت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = اشاره_تهی;
            لكل (خودکار &fmt : fmt_list){
                إذا (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    اكسر;
                }
            }
            إذا (selected_format == اشاره_تهی)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            وضعیت = vaCreateImage(display, selected_format, size.عرض, size.ارتفاع, &image);
            إذا (وضعیت != VA_وضعیت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            وضعیت = vaGetImage(display, surface, 0, 0, size.عرض, size.ارتفاع, image.image_id);
            إذا (وضعیت != VA_وضعیت_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        بیعلامت حرف* میانگیر = 0;
        وضعیت = va_Map_میانگیر(display, image.buf, (فراغ **)&میانگیر);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        إذا (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_مقصد_bgr(image, میانگیر, m);
        إذا (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_مقصد_bgr(image, میانگیر, m);
        وإلا
            CV_Check((صحيح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        وضعیت = va_Unmap_میانگیر(display, image.buf);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        وضعیت = vaDestroyImage(display, image.image_id);
        إذا (وضعیت != VA_وضعیت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#پایان_اگر  // !HAVE_VA
}

}} // namespace cv::va_intel
