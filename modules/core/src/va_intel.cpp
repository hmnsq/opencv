// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#ضمن "precomp.hpp"

#ضمن <opencv2/core/utils/logger.hpp>

#جے_تعریف_شدہ HAVE_VA
#  include <va/va.h>
#وإلا  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#جے_انت // HAVE_VA

ورتوں نطاق cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#جے_تعریف_شدہ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#جے_انت // HAVE_OPENCL

#جے_تعریف_شدہ HAVE_VA_INTEL
#جے_تعریف_شدہ HAVE_VA_INTEL_OLD_سرخی
#  include <CL/va_ext.h>
#وإلا
#  include <CL/cl_va_api_media_sharing_intel.h>
#جے_انت
#جے_انت

#جے_تعریف_شدہ HAVE_VA
#جے_بنا_تعریف OPENCV_LIBVA_LINK
#ضمن "va_wrapper.impl.hpp"
#وإلا
نطاق cv { نطاق detail {
ساكن فراغ init_libva() { /* nothing */ }
}}  // namespace
#جے_انت
ورتوں نطاق cv::detail;
#جے_انت

نطاق cv { نطاق va_intel {

#جے_تعریف_شدہ HAVE_VA_INTEL

صنف VAAPIInterop : عام ocl::Context::UserContext
{
عام:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        cl_قطار_وچ_پاؤ_Acquire_VA_APIMedia_Surfaces_INTEL  = (cl_قطار_وچ_پاؤ_Acquire_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        cl_قطار_وچ_پاؤ_Release_VA_APIMedia_Surfaces_INTEL  = (cl_قطار_وچ_پاؤ_Release_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        إذا (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !cl_قطار_وچ_پاؤ_Acquire_VA_APIMedia_Surfaces_INTEL ||
            !cl_قطار_وچ_پاؤ_Release_VA_APIMedia_Surfaces_INTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    مجازی ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    cl_قطار_وچ_پاؤ_Acquire_VA_APIMedia_Surfaces_INTEL_fn  cl_قطار_وچ_پاؤ_Acquire_VA_APIMedia_Surfaces_INTEL;
    cl_قطار_وچ_پاؤ_Release_VA_APIMedia_Surfaces_INTEL_fn  cl_قطار_وچ_پاؤ_Release_VA_APIMedia_Surfaces_INTEL;
};

#جے_انت // HAVE_VA_INTEL

نطاق ocl {

Context& initializeContextFromVA(VADisplay display, منطقي tryInterop)
{
    CV_بن_ورتیا(display); CV_بن_ورتیا(tryInterop);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    إذا (tryInterop)
    {
        cl_uint عدد_Platforms;
        cl_int حالت = clGetPlatformIDs(0, عدم, &عدد_Platforms);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        إذا (عدد_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(عدد_Platforms);
        حالت = clGetPlatformIDs(عدد_Platforms, &platforms[0], عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        صحيح لبھیا = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        لكل (صحيح i = 0; i < (صحيح)عدد_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            إذا ((فراغ*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == عدم)
            {
                استمر;
            }

            // Query device list

            cl_uint عدد_Devices = 0;

            حالت = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, عدم, &عدد_Devices);
            إذا ((حالت != CL_SUCCESS) || !(عدد_Devices > 0))
                استمر;
            عدد_Devices = 1; // OpenCV expects only 1 device
            حالت = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, عدد_Devices, &device, عدم);
            إذا (حالت != CL_SUCCESS)
                استمر;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, عدد_Devices, &device, عدم, عدم, &حالت);
            إذا (حالت != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            وإلا
            {
                لبھیا = i;
                اكسر;
            }
        }

        إذا (لبھیا >= 0)
        {
            cl_platform_id platform = platforms[لبھیا];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            حاول
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<VAAPIInterop>(platform));
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
#جے_انت  // !HAVE_VA
}

#جے_تعریف_شدہ HAVE_VA_INTEL
ساكن منطقي ocl_convert_nv12_منزل_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_بفر, صحيح قدم, صحيح خانے_گنتی, صحيح صفاں)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلاں(clImageY, clImageUV, cl_بفر, قدم, خانے_گنتی, صفاں);

    size_t globalsize[] = { (size_t)خانے_گنتی/2, (size_t)صفاں/2 };
    أرجع k.لگاتار(2, globalsize, 0, خطأ);
}

ساكن منطقي ocl_convert_bgr_منزل_nv12(cl_mem cl_بفر, صحيح قدم, صحيح خانے_گنتی, صحيح صفاں, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    إذا (k.empty())
        أرجع خطأ;

    k.دلیلاں(cl_بفر, قدم, خانے_گنتی, صفاں, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)خانے_گنتی/2, (size_t)صفاں/2 };
    أرجع k.لگاتار(2, globalsize, 0, خطأ);
}
#جے_انت // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#إذا defined(HAVE_VA)
ثابت صحيح NCHANNELS = 3;

ساكن فراغ copy_convert_nv12_منزل_bgr(ثابت VAImage& image, ثابت بنانشانی حرف* بفر, Mat& bgr)
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

    ثابت size_t src_وتھ_Y = image.offsets[0];
    ثابت size_t src_وتھ_UV = image.offsets[1];

    ثابت size_t src_قدم_Y = image.pitches[0];
    ثابت size_t src_قدم_UV = image.pitches[1];

    ثابت size_t dst_قدم_3 = bgr.قدم;

    ثابت بنانشانی حرف* srcY0 = بفر + src_وتھ_Y;
    ثابت بنانشانی حرف* srcUV = بفر + src_وتھ_UV;

    بنانشانی حرف* dst0 = bgr.انکڑے;

    لكل (صحيح y = 0; y < bgr.صفاں; y += 2)
    {
        ثابت بنانشانی حرف* srcY1 = srcY0 + src_قدم_Y;
        بنانشانی حرف *dst1 = dst0 + dst_قدم_3;

        لكل (صحيح x = 0; x < bgr.خانے_گنتی; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y3 + ruv);
        }

        srcY0 = srcY1 + src_قدم_Y;
        srcUV += src_قدم_UV;
        dst0 = dst1 + dst_قدم_3;
    }
}

ساكن فراغ copy_convert_bgr_منزل_nv12(ثابت VAImage& image, ثابت Mat& bgr, بنانشانی حرف* بفر)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ثابت size_t dst_وتھ_Y = image.offsets[0];
    ثابت size_t dst_وتھ_UV = image.offsets[1];

    ثابت size_t dst_قدم_Y = image.pitches[0];
    ثابت size_t dst_قدم_UV = image.pitches[1];

    ثابت size_t src_قدم_3 = bgr.قدم;

    ثابت بنانشانی حرف* src0 = bgr.انکڑے;

    بنانشانی حرف* dstY0 = بفر + dst_وتھ_Y;
    بنانشانی حرف* dstUV = بفر + dst_وتھ_UV;

    لكل (صحيح y = 0; y < bgr.صفاں; y += 2)
    {
        ثابت بنانشانی حرف *src1 = src0 + src_قدم_3;
        بنانشانی حرف* dstY1 = dstY0 + dst_قدم_Y;

        لكل (صحيح x = 0; x < bgr.خانے_گنتی; x += 2)
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

            dstY0[x+0] = saturate_cast<بنانشانی حرف>(Y0);
            dstY0[x+1] = saturate_cast<بنانشانی حرف>(Y1);
            dstY1[x+0] = saturate_cast<بنانشانی حرف>(Y2);
            dstY1[x+1] = saturate_cast<بنانشانی حرف>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<بنانشانی حرف>(U);
            dstUV[2*(x/2)+1] = saturate_cast<بنانشانی حرف>(V);
        }

        src0 = src1 + src_قدم_3;
        dstY0 = dstY1 + dst_قدم_Y;
        dstUV += dst_قدم_UV;
    }
}


ساكن فراغ copy_convert_yv12_منزل_bgr(ثابت VAImage& image, ثابت بنانشانی حرف* بفر, Mat& bgr)
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
    CV_CheckEQ((size_t)image.عدد_planes, (size_t)3, "");

    ثابت size_t src_وتھ_Y = image.offsets[0];
    ثابت size_t src_وتھ_V = image.offsets[1];
    ثابت size_t src_وتھ_U = image.offsets[2];

    ثابت size_t src_قدم_Y = image.pitches[0];
    ثابت size_t src_قدم_U = image.pitches[1];
    ثابت size_t src_قدم_V = image.pitches[2];

    ثابت size_t dst_قدم_3 = bgr.قدم;

    ثابت بنانشانی حرف* srcY_ = بفر + src_وتھ_Y;
    ثابت بنانشانی حرف* srcV_ = بفر + src_وتھ_V;
    ثابت بنانشانی حرف* srcU_ = بفر + src_وتھ_U;

    لكل (صحيح y = 0; y < bgr.صفاں; y += 2)
    {
        ثابت بنانشانی حرف* srcY0 = srcY_ + (src_قدم_Y) * y;
        ثابت بنانشانی حرف* srcY1 = srcY0 + src_قدم_Y;

        ثابت بنانشانی حرف* srcV = srcV_ + (src_قدم_V) * y / 2;
        ثابت بنانشانی حرف* srcU = srcU_ + (src_قدم_U) * y / 2;

        بنانشانی حرف* dst0 = bgr.انکڑے + (dst_قدم_3) * y;
        بنانشانی حرف* dst1 = dst0 + dst_قدم_3;

        لكل (صحيح x = 0; x < bgr.خانے_گنتی; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<بنانشانی حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<بنانشانی حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<بنانشانی حرف>(Y3 + ruv);
        }
    }
}

ساكن فراغ copy_convert_bgr_منزل_yv12(ثابت VAImage& image, ثابت Mat& bgr, بنانشانی حرف* بفر)
{
    ثابت عائم d1 = 16.0f;
    ثابت عائم d2 = 128.0f;

    ساكن ثابت عائم coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.عدد_planes, (size_t)3, "");

    ثابت size_t dst_وتھ_Y = image.offsets[0];
    ثابت size_t dst_وتھ_V = image.offsets[1];
    ثابت size_t dst_وتھ_U = image.offsets[2];

    ثابت size_t dst_قدم_Y = image.pitches[0];
    ثابت size_t dst_قدم_U = image.pitches[1];
    ثابت size_t dst_قدم_V = image.pitches[2];

    بنانشانی حرف* dstY_ = بفر + dst_وتھ_Y;
    بنانشانی حرف* dstV_ = بفر + dst_وتھ_V;
    بنانشانی حرف* dstU_ = بفر + dst_وتھ_U;

    ثابت size_t src_قدم_3 = bgr.قدم;

    لكل (صحيح y = 0; y < bgr.صفاں; y += 2)
    {
        بنانشانی حرف* dstY0 = dstY_ + (dst_قدم_Y) * y;
        بنانشانی حرف* dstY1 = dstY0 + dst_قدم_Y;

        بنانشانی حرف* dstV = dstV_ + (dst_قدم_V) * y / 2;
        بنانشانی حرف* dstU = dstU_ + (dst_قدم_U) * y / 2;

        ثابت بنانشانی حرف* src0 = bgr.انکڑے + (src_قدم_3) * y;
        ثابت بنانشانی حرف* src1 = src0 + src_قدم_3;

        لكل (صحيح x = 0; x < bgr.خانے_گنتی; x += 2)
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

            dstY0[x+0] = saturate_cast<بنانشانی حرف>(Y0);
            dstY0[x+1] = saturate_cast<بنانشانی حرف>(Y1);
            dstY1[x+0] = saturate_cast<بنانشانی حرف>(Y2);
            dstY1[x+1] = saturate_cast<بنانشانی حرف>(Y3);

            dstU[x/2] = saturate_cast<بنانشانی حرف>(U);
            dstV[x/2] = saturate_cast<بنانشانی حرف>(V);
        }
    }
}
#جے_انت // HAVE_VA

فراغ convert_منزل_VASurface(VADisplay display, داخلہ_Array src, VASurfaceID surface, ناپ size)
{
    CV_بن_ورتیا(display); CV_بن_ورتیا(src); CV_بن_ورتیا(surface); CV_بن_ورتیا(size);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

    ثابت صحيح stype = CV_8UC3;

    صحيح srcType = src.type();
    CV_Assert(srcType == stype);

    ناپ src_ناپ = src.size();
    CV_Assert(src_ناپ.چوڑائی == size.چوڑائی && src_ناپ.اچائی == size.اچائی);

#جے_تعریف_شدہ HAVE_VA_INTEL
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

        cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int حالت = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_قطار q = (cl_command_قطار)ocl_context.get_قطار().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        حالت = interop->cl_قطار_وچ_پاؤ_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        إذا (!ocl::ocl_convert_bgr_منزل_nv12(cl_بفر, (صحيح)u.قدم[0], u.خانے_گنتی, u.صفاں, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->cl_قطار_وچ_پاؤ_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        حالت = cl_انت(q); // TODO Use events
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        حالت = clReleaseMemObject(clImageY); // TODO RAII
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        حالت = clReleaseMemObject(clImageUV);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وإلا
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.انکڑے == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus حالت = 0;

        حالت = vaSyncSurface(display, surface);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        منطقي اسدھا_بفر = خطأ;
        VAImage image;
        حالت = vaDeriveImage(display, surface, &image);
        إذا (حالت != VA_حالت_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            اسدھا_بفر = سچ;
            صحيح عدد_formats = va_Max_عدد_Image_Formats(display);
            إذا (عدد_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(عدد_formats);

            حالت = vaQueryImageFormats(display, fmt_list.انکڑے(), &عدد_formats);
            إذا (حالت != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = خالی_سوچک;
            لكل (خودکار &fmt : fmt_list){
                إذا (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    اكسر;
                }
            }
            إذا (selected_format == خالی_سوچک)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            حالت = vaCreateImage(display, selected_format, size.چوڑائی, size.اچائی, &image);
            إذا (حالت != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        بنانشانی حرف* بفر = 0;
        حالت = va_Map_بفر(display, image.buf, (فراغ **)&بفر);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        إذا (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_منزل_nv12(image, m, بفر);
        إذا (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_منزل_yv12(image, m, بفر);
        وإلا
            CV_Check((صحيح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        حالت = va_Unmap_بفر(display, image.buf);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        إذا (اسدھا_بفر){
            حالت = vaPutImage(display, surface, image.image_id, 0, 0, size.چوڑائی, size.اچائی, 0, 0, size.چوڑائی, size.اچائی);
            إذا (حالت != VA_حالت_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        حالت = vaDestroyImage(display, image.image_id);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#جے_انت  // !HAVE_VA
}

فراغ convertFromVASurface(VADisplay display, VASurfaceID surface, ناپ size, OutputArray dst)
{
    CV_بن_ورتیا(display); CV_بن_ورتیا(surface); CV_بن_ورتیا(dst); CV_بن_ورتیا(size);
#إذا !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#وإلا  // !HAVE_VA

    ثابت صحيح dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#جے_تعریف_شدہ HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    إذا (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int حالت = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &حالت);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_قطار q = (cl_command_قطار)ocl_context.get_قطار().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        حالت = interop->cl_قطار_وچ_پاؤ_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        إذا (!ocl::ocl_convert_nv12_منزل_bgr(clImageY, clImageUV, cl_بفر, (صحيح)u.قدم[0], u.خانے_گنتی, u.صفاں))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        حالت = interop->cl_قطار_وچ_پاؤ_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, عدم, عدم);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        حالت = cl_انت(q); // TODO Use events
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        حالت = clReleaseMemObject(clImageY); // TODO RAII
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        حالت = clReleaseMemObject(clImageUV);
        إذا (حالت != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    وإلا
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.انکڑے == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus حالت = 0;

        حالت = vaSyncSurface(display, surface);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        حالت = vaDeriveImage(display, surface, &image);
        إذا (حالت != VA_حالت_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            صحيح عدد_formats = va_Max_عدد_Image_Formats(display);
            إذا (عدد_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(عدد_formats);

            حالت = vaQueryImageFormats(display, fmt_list.انکڑے(), &عدد_formats);
            إذا (حالت != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = خالی_سوچک;
            لكل (خودکار &fmt : fmt_list){
                إذا (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    اكسر;
                }
            }
            إذا (selected_format == خالی_سوچک)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            حالت = vaCreateImage(display, selected_format, size.چوڑائی, size.اچائی, &image);
            إذا (حالت != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            حالت = vaGetImage(display, surface, 0, 0, size.چوڑائی, size.اچائی, image.image_id);
            إذا (حالت != VA_حالت_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        بنانشانی حرف* بفر = 0;
        حالت = va_Map_بفر(display, image.buf, (فراغ **)&بفر);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        إذا (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_منزل_bgr(image, بفر, m);
        إذا (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_منزل_bgr(image, بفر, m);
        وإلا
            CV_Check((صحيح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        حالت = va_Unmap_بفر(display, image.buf);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        حالت = vaDestroyImage(display, image.image_id);
        إذا (حالت != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#جے_انت  // !HAVE_VA
}

}} // namespace cv::va_intel
