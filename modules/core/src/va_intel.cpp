// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#شامل "precomp.hpp"

#شامل <opencv2/core/utils/logger.hpp>

#اگر_تعریف_شدہ HAVE_VA
#  include <va/va.h>
#ورنہ  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#اگر_ختم // HAVE_VA

استعمال نامخلا cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#اگر_تعریف_شدہ HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#اگر_ختم // HAVE_OPENCL

#اگر_تعریف_شدہ HAVE_VA_INTEL
#اگر_تعریف_شدہ HAVE_VA_INTEL_OLD_سرخی
#  include <CL/va_ext.h>
#ورنہ
#  include <CL/cl_va_api_media_sharing_intel.h>
#اگر_ختم
#اگر_ختم

#اگر_تعریف_شدہ HAVE_VA
#اگر_غیر_تعریف_شدہ OPENCV_LIBVA_LINK
#شامل "va_wrapper.impl.hpp"
#ورنہ
نامخلا cv { نامخلا detail {
ساکن خالی init_libva() { /* nothing */ }
}}  // namespace
#اگر_ختم
استعمال نامخلا cv::detail;
#اگر_ختم

نامخلا cv { نامخلا va_intel {

#اگر_تعریف_شدہ HAVE_VA_INTEL

جماعت VAAPIInterop : عوامی ocl::Context::UserContext
{
عوامی:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        cl_قطار_میں_ڈالو_Acquire_VA_APIMedia_Surfaces_INTEL  = (cl_قطار_میں_ڈالو_Acquire_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        cl_قطار_میں_ڈالو_Release_VA_APIMedia_Surfaces_INTEL  = (cl_قطار_میں_ڈالو_Release_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        اگر (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !cl_قطار_میں_ڈالو_Acquire_VA_APIMedia_Surfaces_INTEL ||
            !cl_قطار_میں_ڈالو_Release_VA_APIMedia_Surfaces_INTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    مجازی ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    cl_قطار_میں_ڈالو_Acquire_VA_APIMedia_Surfaces_INTEL_fn  cl_قطار_میں_ڈالو_Acquire_VA_APIMedia_Surfaces_INTEL;
    cl_قطار_میں_ڈالو_Release_VA_APIMedia_Surfaces_INTEL_fn  cl_قطار_میں_ڈالو_Release_VA_APIMedia_Surfaces_INTEL;
};

#اگر_ختم // HAVE_VA_INTEL

نامخلا ocl {

Context& initializeContextFromVA(VADisplay display, منطقی tryInterop)
{
    CV_غیر_مستعمل(display); CV_غیر_مستعمل(tryInterop);
#اگر !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ورنہ  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    اگر (tryInterop)
    {
        cl_uint عدد_Platforms;
        cl_int حالت_2 = clGetPlatformIDs(0, تہی, &عدد_Platforms);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        اگر (عدد_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(عدد_Platforms);
        حالت_2 = clGetPlatformIDs(عدد_Platforms, &platforms[0], تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        صحیح ملا = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        برائے (صحیح i = 0; i < (صحیح)عدد_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                cl_Get_Extension_Function_پتہ_For_Platform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            اگر ((خالی*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == تہی)
            {
                جاری;
            }

            // Query device list

            cl_uint عدد_Devices = 0;

            حالت_2 = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, تہی, &عدد_Devices);
            اگر ((حالت_2 != CL_SUCCESS) || !(عدد_Devices > 0))
                جاری;
            عدد_Devices = 1; // OpenCV expects only 1 device
            حالت_2 = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, عدد_Devices, &device, تہی);
            اگر (حالت_2 != CL_SUCCESS)
                جاری;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, عدد_Devices, &device, تہی, تہی, &حالت_2);
            اگر (حالت_2 != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            ورنہ
            {
                ملا = i;
                توڑو;
            }
        }

        اگر (ملا >= 0)
        {
            cl_platform_id platform = platforms[ملا];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            کوشش
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().سیٹ_User_Context(std::make_shared<VAAPIInterop>(platform));
            }
            پکڑو (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                پھینکو;
            }
            clExecCtx.bind();
            واپس const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        واپس ctx;
    }
#اگر_ختم  // !HAVE_VA
}

#اگر_تعریف_شدہ HAVE_VA_INTEL
ساکن منطقی ocl_convert_nv12_مقصد_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_بفر, صحیح قدم, صحیح ستون_تعداد, صحیح صفیں)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(clImageY, clImageUV, cl_بفر, قدم, ستون_تعداد, صفیں);

    size_t globalsize[] = { (size_t)ستون_تعداد/2, (size_t)صفیں/2 };
    واپس k.تسلسل(2, globalsize, 0, جھوٹ);
}

ساکن منطقی ocl_convert_bgr_مقصد_nv12(cl_mem cl_بفر, صحیح قدم, صحیح ستون_تعداد, صحیح صفیں, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    اگر (k.empty())
        واپس جھوٹ;

    k.دلائل(cl_بفر, قدم, ستون_تعداد, صفیں, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)ستون_تعداد/2, (size_t)صفیں/2 };
    واپس k.تسلسل(2, globalsize, 0, جھوٹ);
}
#اگر_ختم // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#اگر defined(HAVE_VA)
ثابت صحیح NCHANNELS = 3;

ساکن خالی copy_convert_nv12_مقصد_bgr(ثابت VAImage& image, ثابت غیرعلامتی حرف* بفر, Mat& bgr)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ساکن ثابت اعشاری coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    ثابت size_t src_انحراف_Y = image.offsets[0];
    ثابت size_t src_انحراف_UV = image.offsets[1];

    ثابت size_t src_قدم_Y = image.pitches[0];
    ثابت size_t src_قدم_UV = image.pitches[1];

    ثابت size_t dst_قدم_3 = bgr.قدم;

    ثابت غیرعلامتی حرف* srcY0 = بفر + src_انحراف_Y;
    ثابت غیرعلامتی حرف* srcUV = بفر + src_انحراف_UV;

    غیرعلامتی حرف* dst0 = bgr.ڈیٹا;

    برائے (صحیح y = 0; y < bgr.صفیں; y += 2)
    {
        ثابت غیرعلامتی حرف* srcY1 = srcY0 + src_قدم_Y;
        غیرعلامتی حرف *dst1 = dst0 + dst_قدم_3;

        برائے (صحیح x = 0; x < bgr.ستون_تعداد; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y3 + ruv);
        }

        srcY0 = srcY1 + src_قدم_Y;
        srcUV += src_قدم_UV;
        dst0 = dst1 + dst_قدم_3;
    }
}

ساکن خالی copy_convert_bgr_مقصد_nv12(ثابت VAImage& image, ثابت Mat& bgr, غیرعلامتی حرف* بفر)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ساکن ثابت اعشاری coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ثابت size_t dst_انحراف_Y = image.offsets[0];
    ثابت size_t dst_انحراف_UV = image.offsets[1];

    ثابت size_t dst_قدم_Y = image.pitches[0];
    ثابت size_t dst_قدم_UV = image.pitches[1];

    ثابت size_t src_قدم_3 = bgr.قدم;

    ثابت غیرعلامتی حرف* src0 = bgr.ڈیٹا;

    غیرعلامتی حرف* dstY0 = بفر + dst_انحراف_Y;
    غیرعلامتی حرف* dstUV = بفر + dst_انحراف_UV;

    برائے (صحیح y = 0; y < bgr.صفیں; y += 2)
    {
        ثابت غیرعلامتی حرف *src1 = src0 + src_قدم_3;
        غیرعلامتی حرف* dstY1 = dstY0 + dst_قدم_Y;

        برائے (صحیح x = 0; x < bgr.ستون_تعداد; x += 2)
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

            dstY0[x+0] = saturate_cast<غیرعلامتی حرف>(Y0);
            dstY0[x+1] = saturate_cast<غیرعلامتی حرف>(Y1);
            dstY1[x+0] = saturate_cast<غیرعلامتی حرف>(Y2);
            dstY1[x+1] = saturate_cast<غیرعلامتی حرف>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<غیرعلامتی حرف>(U);
            dstUV[2*(x/2)+1] = saturate_cast<غیرعلامتی حرف>(V);
        }

        src0 = src1 + src_قدم_3;
        dstY0 = dstY1 + dst_قدم_Y;
        dstUV += dst_قدم_UV;
    }
}


ساکن خالی copy_convert_yv12_مقصد_bgr(ثابت VAImage& image, ثابت غیرعلامتی حرف* بفر, Mat& bgr)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ساکن ثابت اعشاری coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.عدد_planes, (size_t)3, "");

    ثابت size_t src_انحراف_Y = image.offsets[0];
    ثابت size_t src_انحراف_V = image.offsets[1];
    ثابت size_t src_انحراف_U = image.offsets[2];

    ثابت size_t src_قدم_Y = image.pitches[0];
    ثابت size_t src_قدم_U = image.pitches[1];
    ثابت size_t src_قدم_V = image.pitches[2];

    ثابت size_t dst_قدم_3 = bgr.قدم;

    ثابت غیرعلامتی حرف* srcY_ = بفر + src_انحراف_Y;
    ثابت غیرعلامتی حرف* srcV_ = بفر + src_انحراف_V;
    ثابت غیرعلامتی حرف* srcU_ = بفر + src_انحراف_U;

    برائے (صحیح y = 0; y < bgr.صفیں; y += 2)
    {
        ثابت غیرعلامتی حرف* srcY0 = srcY_ + (src_قدم_Y) * y;
        ثابت غیرعلامتی حرف* srcY1 = srcY0 + src_قدم_Y;

        ثابت غیرعلامتی حرف* srcV = srcV_ + (src_قدم_V) * y / 2;
        ثابت غیرعلامتی حرف* srcU = srcU_ + (src_قدم_U) * y / 2;

        غیرعلامتی حرف* dst0 = bgr.ڈیٹا + (dst_قدم_3) * y;
        غیرعلامتی حرف* dst1 = dst0 + dst_قدم_3;

        برائے (صحیح x = 0; x < bgr.ستون_تعداد; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<غیرعلامتی حرف>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<غیرعلامتی حرف>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<غیرعلامتی حرف>(Y3 + ruv);
        }
    }
}

ساکن خالی copy_convert_bgr_مقصد_yv12(ثابت VAImage& image, ثابت Mat& bgr, غیرعلامتی حرف* بفر)
{
    ثابت اعشاری d1 = 16.0f;
    ثابت اعشاری d2 = 128.0f;

    ساکن ثابت اعشاری coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.عدد_planes, (size_t)3, "");

    ثابت size_t dst_انحراف_Y = image.offsets[0];
    ثابت size_t dst_انحراف_V = image.offsets[1];
    ثابت size_t dst_انحراف_U = image.offsets[2];

    ثابت size_t dst_قدم_Y = image.pitches[0];
    ثابت size_t dst_قدم_U = image.pitches[1];
    ثابت size_t dst_قدم_V = image.pitches[2];

    غیرعلامتی حرف* dstY_ = بفر + dst_انحراف_Y;
    غیرعلامتی حرف* dstV_ = بفر + dst_انحراف_V;
    غیرعلامتی حرف* dstU_ = بفر + dst_انحراف_U;

    ثابت size_t src_قدم_3 = bgr.قدم;

    برائے (صحیح y = 0; y < bgr.صفیں; y += 2)
    {
        غیرعلامتی حرف* dstY0 = dstY_ + (dst_قدم_Y) * y;
        غیرعلامتی حرف* dstY1 = dstY0 + dst_قدم_Y;

        غیرعلامتی حرف* dstV = dstV_ + (dst_قدم_V) * y / 2;
        غیرعلامتی حرف* dstU = dstU_ + (dst_قدم_U) * y / 2;

        ثابت غیرعلامتی حرف* src0 = bgr.ڈیٹا + (src_قدم_3) * y;
        ثابت غیرعلامتی حرف* src1 = src0 + src_قدم_3;

        برائے (صحیح x = 0; x < bgr.ستون_تعداد; x += 2)
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

            dstY0[x+0] = saturate_cast<غیرعلامتی حرف>(Y0);
            dstY0[x+1] = saturate_cast<غیرعلامتی حرف>(Y1);
            dstY1[x+0] = saturate_cast<غیرعلامتی حرف>(Y2);
            dstY1[x+1] = saturate_cast<غیرعلامتی حرف>(Y3);

            dstU[x/2] = saturate_cast<غیرعلامتی حرف>(U);
            dstV[x/2] = saturate_cast<غیرعلامتی حرف>(V);
        }
    }
}
#اگر_ختم // HAVE_VA

خالی convert_مقصد_VASurface(VADisplay display, داخلہ_Array src, VASurfaceID surface, سائز size)
{
    CV_غیر_مستعمل(display); CV_غیر_مستعمل(src); CV_غیر_مستعمل(surface); CV_غیر_مستعمل(size);
#اگر !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ورنہ  // !HAVE_VA

    ثابت صحیح stype = CV_8UC3;

    صحیح srcType = src.type();
    CV_Assert(srcType == stype);

    سائز src_سائز = src.size();
    CV_Assert(src_سائز.چوڑائی == size.چوڑائی && src_سائز.اونچائی == size.اونچائی);

#اگر_تعریف_شدہ HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(تہی, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    خالی* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(تہی, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    منطقی isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(تہی, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (خالی*)display << "(surface)");
    اگر (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int حالت_2 = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_قطار q = (cl_command_قطار)ocl_context.get_قطار().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        حالت_2 = interop->cl_قطار_میں_ڈالو_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        اگر (!ocl::ocl_convert_bgr_مقصد_nv12(cl_بفر, (صحیح)u.قدم[0], u.ستون_تعداد, u.صفیں, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->cl_قطار_میں_ڈالو_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        حالت_2 = cl_اختتام(q); // TODO Use events
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        حالت_2 = clReleaseMemObject(clImageY); // TODO RAII
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        حالت_2 = clReleaseMemObject(clImageUV);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ورنہ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.ڈیٹا == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus حالت_2 = 0;

        حالت_2 = vaSyncSurface(display, surface);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        منطقی بالواسطہ_بفر = جھوٹ;
        VAImage image;
        حالت_2 = vaDeriveImage(display, surface, &image);
        اگر (حالت_2 != VA_حالت_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            بالواسطہ_بفر = سچ;
            صحیح عدد_formats = va_Max_عدد_Image_Formats(display);
            اگر (عدد_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(عدد_formats);

            حالت_2 = vaQueryImageFormats(display, fmt_list.ڈیٹا(), &عدد_formats);
            اگر (حالت_2 != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = خالی_اشاریہ;
            برائے (خودکار &fmt : fmt_list){
                اگر (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    توڑو;
                }
            }
            اگر (selected_format == خالی_اشاریہ)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            حالت_2 = vaCreateImage(display, selected_format, size.چوڑائی, size.اونچائی, &image);
            اگر (حالت_2 != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        غیرعلامتی حرف* بفر = 0;
        حالت_2 = va_Map_بفر(display, image.buf, (خالی **)&بفر);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        اگر (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_مقصد_nv12(image, m, بفر);
        اگر (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_مقصد_yv12(image, m, بفر);
        ورنہ
            CV_Check((صحیح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        حالت_2 = va_Unmap_بفر(display, image.buf);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        اگر (بالواسطہ_بفر){
            حالت_2 = vaPutImage(display, surface, image.image_id, 0, 0, size.چوڑائی, size.اونچائی, 0, 0, size.چوڑائی, size.اونچائی);
            اگر (حالت_2 != VA_حالت_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        حالت_2 = vaDestroyImage(display, image.image_id);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#اگر_ختم  // !HAVE_VA
}

خالی convertFromVASurface(VADisplay display, VASurfaceID surface, سائز size, OutputArray dst)
{
    CV_غیر_مستعمل(display); CV_غیر_مستعمل(surface); CV_غیر_مستعمل(dst); CV_غیر_مستعمل(size);
#اگر !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#ورنہ  // !HAVE_VA

    ثابت صحیح dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#اگر_تعریف_شدہ HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    اگر (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_بفر = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int حالت_2 = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &حالت_2);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_قطار q = (cl_command_قطار)ocl_context.get_قطار().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        حالت_2 = interop->cl_قطار_میں_ڈالو_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        اگر (!ocl::ocl_convert_nv12_مقصد_bgr(clImageY, clImageUV, cl_بفر, (صحیح)u.قدم[0], u.ستون_تعداد, u.صفیں))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        حالت_2 = interop->cl_قطار_میں_ڈالو_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, تہی, تہی);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        حالت_2 = cl_اختتام(q); // TODO Use events
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        حالت_2 = clReleaseMemObject(clImageY); // TODO RAII
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        حالت_2 = clReleaseMemObject(clImageUV);
        اگر (حالت_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    ورنہ
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.ڈیٹا == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus حالت_2 = 0;

        حالت_2 = vaSyncSurface(display, surface);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        حالت_2 = vaDeriveImage(display, surface, &image);
        اگر (حالت_2 != VA_حالت_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            صحیح عدد_formats = va_Max_عدد_Image_Formats(display);
            اگر (عدد_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(عدد_formats);

            حالت_2 = vaQueryImageFormats(display, fmt_list.ڈیٹا(), &عدد_formats);
            اگر (حالت_2 != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = خالی_اشاریہ;
            برائے (خودکار &fmt : fmt_list){
                اگر (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    توڑو;
                }
            }
            اگر (selected_format == خالی_اشاریہ)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            حالت_2 = vaCreateImage(display, selected_format, size.چوڑائی, size.اونچائی, &image);
            اگر (حالت_2 != VA_حالت_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            حالت_2 = vaGetImage(display, surface, 0, 0, size.چوڑائی, size.اونچائی, image.image_id);
            اگر (حالت_2 != VA_حالت_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        غیرعلامتی حرف* بفر = 0;
        حالت_2 = va_Map_بفر(display, image.buf, (خالی **)&بفر);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        اگر (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_مقصد_bgr(image, بفر, m);
        اگر (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_مقصد_bgr(image, بفر, m);
        ورنہ
            CV_Check((صحیح)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        حالت_2 = va_Unmap_بفر(display, image.buf);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        حالت_2 = vaDestroyImage(display, image.image_id);
        اگر (حالت_2 != VA_حالت_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#اگر_ختم  // !HAVE_VA
}

}} // namespace cv::va_intel
