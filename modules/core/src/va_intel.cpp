// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#अन्तर्भाव "precomp.hpp"

#अन्तर्भाव <opencv2/core/utils/logger.hpp>

#यदि_परिभाषितम् HAVE_VA
#  include <va/va.h>
#अन्यथा  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#यदि_अन्तः // HAVE_VA

उपयुज्य नामाकाश cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#यदि_परिभाषितम् HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#यदि_अन्तः // HAVE_OPENCL

#यदि_परिभाषितम् HAVE_VA_INTEL
#यदि_परिभाषितम् HAVE_VA_INTEL_OLD_शीर्षकम्
#  include <CL/va_ext.h>
#अन्यथा
#  include <CL/cl_va_api_media_sharing_intel.h>
#यदि_अन्तः
#यदि_अन्तः

#यदि_परिभाषितम् HAVE_VA
#यदि_अपरिभाषितम् OPENCV_LIBVA_LINK
#अन्तर्भाव "va_wrapper.impl.hpp"
#अन्यथा
नामाकाश cv { नामाकाश detail {
स्थिर शून्य init_libva() { /* nothing */ }
}}  // namespace
#यदि_अन्तः
उपयुज्य नामाकाश cv::detail;
#यदि_अन्तः

नामाकाश cv { नामाकाश va_intel {

#यदि_परिभाषितम् HAVE_VA_INTEL

वर्ग VAAPIInterop : सार्वजनिक ocl::Context::UserContext
{
सार्वजनिक:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        cl_पङ्क्तौ_निक्षिप_Acquire_VA_APIMedia_Surfaces_INTEL  = (cl_पङ्क्तौ_निक्षिप_Acquire_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        cl_पङ्क्तौ_निक्षिप_Release_VA_APIMedia_Surfaces_INTEL  = (cl_पङ्क्तौ_निक्षिप_Release_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        यदि (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !cl_पङ्क्तौ_निक्षिप_Acquire_VA_APIMedia_Surfaces_INTEL ||
            !cl_पङ्क्तौ_निक्षिप_Release_VA_APIMedia_Surfaces_INTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    आभास ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    cl_पङ्क्तौ_निक्षिप_Acquire_VA_APIMedia_Surfaces_INTEL_fn  cl_पङ्क्तौ_निक्षिप_Acquire_VA_APIMedia_Surfaces_INTEL;
    cl_पङ्क्तौ_निक्षिप_Release_VA_APIMedia_Surfaces_INTEL_fn  cl_पङ्क्तौ_निक्षिप_Release_VA_APIMedia_Surfaces_INTEL;
};

#यदि_अन्तः // HAVE_VA_INTEL

नामाकाश ocl {

Context& initializeContextFromVA(VADisplay display, तार्किक tryInterop)
{
    CV_अप्रयुक्तम्(display); CV_अप्रयुक्तम्(tryInterop);
#यदि !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    यदि (tryInterop)
    {
        cl_uint अङ्कः_Platforms;
        cl_int स्थितिः = clGetPlatformIDs(0, रिक्त, &अङ्कः_Platforms);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        यदि (अङ्कः_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(अङ्कः_Platforms);
        स्थितिः = clGetPlatformIDs(अङ्कः_Platforms, &platforms[0], रिक्त);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        पूर्णाङ्क प्राप्तम् = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        प्रति (पूर्णाङ्क i = 0; i < (पूर्णाङ्क)अङ्कः_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                cl_Get_Extension_Function_स्थानाङ्कः_For_Platform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            यदि ((शून्य*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == रिक्त)
            {
                अनुवर्त;
            }

            // Query device list

            cl_uint अङ्कः_Devices = 0;

            स्थितिः = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, रिक्त, &अङ्कः_Devices);
            यदि ((स्थितिः != CL_SUCCESS) || !(अङ्कः_Devices > 0))
                अनुवर्त;
            अङ्कः_Devices = 1; // OpenCV expects only 1 device
            स्थितिः = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, अङ्कः_Devices, &device, रिक्त);
            यदि (स्थितिः != CL_SUCCESS)
                अनुवर्त;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, अङ्कः_Devices, &device, रिक्त, रिक्त, &स्थितिः);
            यदि (स्थितिः != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            अन्यथा
            {
                प्राप्तम् = i;
                भङ्ग;
            }
        }

        यदि (प्राप्तम् >= 0)
        {
            cl_platform_id platform = platforms[प्राप्तम्];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            यत्न
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().समुच्चयः_User_Context(std::make_shared<VAAPIInterop>(platform));
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
#यदि_अन्तः  // !HAVE_VA
}

#यदि_परिभाषितम् HAVE_VA_INTEL
स्थिर तार्किक ocl_convert_nv12_गन्तव्यम्_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_बफर, पूर्णाङ्क चरणः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क पङ्क्तिकाः)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.तर्काः(clImageY, clImageUV, cl_बफर, चरणः, स्तम्भसङ्ख्या, पङ्क्तिकाः);

    size_t globalsize[] = { (size_t)स्तम्भसङ्ख्या/2, (size_t)पङ्क्तिकाः/2 };
    प्रत्यागम k.सततम्(2, globalsize, 0, असत्य);
}

स्थिर तार्किक ocl_convert_bgr_गन्तव्यम्_nv12(cl_mem cl_बफर, पूर्णाङ्क चरणः, पूर्णाङ्क स्तम्भसङ्ख्या, पूर्णाङ्क पङ्क्तिकाः, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    यदि (k.empty())
        प्रत्यागम असत्य;

    k.तर्काः(cl_बफर, चरणः, स्तम्भसङ्ख्या, पङ्क्तिकाः, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)स्तम्भसङ्ख्या/2, (size_t)पङ्क्तिकाः/2 };
    प्रत्यागम k.सततम्(2, globalsize, 0, असत्य);
}
#यदि_अन्तः // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#यदि defined(HAVE_VA)
ध्रुव पूर्णाङ्क NCHANNELS = 3;

स्थिर शून्य copy_convert_nv12_गन्तव्यम्_bgr(ध्रुव VAImage& image, ध्रुव अचिह्नित वर्ण* बफर, Mat& bgr)
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

    ध्रुव size_t src_विस्थापनम्_Y = image.offsets[0];
    ध्रुव size_t src_विस्थापनम्_UV = image.offsets[1];

    ध्रुव size_t src_चरणः_Y = image.pitches[0];
    ध्रुव size_t src_चरणः_UV = image.pitches[1];

    ध्रुव size_t dst_चरणः_3 = bgr.चरणः;

    ध्रुव अचिह्नित वर्ण* srcY0 = बफर + src_विस्थापनम्_Y;
    ध्रुव अचिह्नित वर्ण* srcUV = बफर + src_विस्थापनम्_UV;

    अचिह्नित वर्ण* dst0 = bgr.दत्त;

    प्रति (पूर्णाङ्क y = 0; y < bgr.पङ्क्तिकाः; y += 2)
    {
        ध्रुव अचिह्नित वर्ण* srcY1 = srcY0 + src_चरणः_Y;
        अचिह्नित वर्ण *dst1 = dst0 + dst_चरणः_3;

        प्रति (पूर्णाङ्क x = 0; x < bgr.स्तम्भसङ्ख्या; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y3 + ruv);
        }

        srcY0 = srcY1 + src_चरणः_Y;
        srcUV += src_चरणः_UV;
        dst0 = dst1 + dst_चरणः_3;
    }
}

स्थिर शून्य copy_convert_bgr_गन्तव्यम्_nv12(ध्रुव VAImage& image, ध्रुव Mat& bgr, अचिह्नित वर्ण* बफर)
{
    ध्रुव भिन्न d1 = 16.0f;
    ध्रुव भिन्न d2 = 128.0f;

    स्थिर ध्रुव भिन्न coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    ध्रुव size_t dst_विस्थापनम्_Y = image.offsets[0];
    ध्रुव size_t dst_विस्थापनम्_UV = image.offsets[1];

    ध्रुव size_t dst_चरणः_Y = image.pitches[0];
    ध्रुव size_t dst_चरणः_UV = image.pitches[1];

    ध्रुव size_t src_चरणः_3 = bgr.चरणः;

    ध्रुव अचिह्नित वर्ण* src0 = bgr.दत्त;

    अचिह्नित वर्ण* dstY0 = बफर + dst_विस्थापनम्_Y;
    अचिह्नित वर्ण* dstUV = बफर + dst_विस्थापनम्_UV;

    प्रति (पूर्णाङ्क y = 0; y < bgr.पङ्क्तिकाः; y += 2)
    {
        ध्रुव अचिह्नित वर्ण *src1 = src0 + src_चरणः_3;
        अचिह्नित वर्ण* dstY1 = dstY0 + dst_चरणः_Y;

        प्रति (पूर्णाङ्क x = 0; x < bgr.स्तम्भसङ्ख्या; x += 2)
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

            dstY0[x+0] = saturate_cast<अचिह्नित वर्ण>(Y0);
            dstY0[x+1] = saturate_cast<अचिह्नित वर्ण>(Y1);
            dstY1[x+0] = saturate_cast<अचिह्नित वर्ण>(Y2);
            dstY1[x+1] = saturate_cast<अचिह्नित वर्ण>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<अचिह्नित वर्ण>(U);
            dstUV[2*(x/2)+1] = saturate_cast<अचिह्नित वर्ण>(V);
        }

        src0 = src1 + src_चरणः_3;
        dstY0 = dstY1 + dst_चरणः_Y;
        dstUV += dst_चरणः_UV;
    }
}


स्थिर शून्य copy_convert_yv12_गन्तव्यम्_bgr(ध्रुव VAImage& image, ध्रुव अचिह्नित वर्ण* बफर, Mat& bgr)
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

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.अङ्कः_planes, (size_t)3, "");

    ध्रुव size_t src_विस्थापनम्_Y = image.offsets[0];
    ध्रुव size_t src_विस्थापनम्_V = image.offsets[1];
    ध्रुव size_t src_विस्थापनम्_U = image.offsets[2];

    ध्रुव size_t src_चरणः_Y = image.pitches[0];
    ध्रुव size_t src_चरणः_U = image.pitches[1];
    ध्रुव size_t src_चरणः_V = image.pitches[2];

    ध्रुव size_t dst_चरणः_3 = bgr.चरणः;

    ध्रुव अचिह्नित वर्ण* srcY_ = बफर + src_विस्थापनम्_Y;
    ध्रुव अचिह्नित वर्ण* srcV_ = बफर + src_विस्थापनम्_V;
    ध्रुव अचिह्नित वर्ण* srcU_ = बफर + src_विस्थापनम्_U;

    प्रति (पूर्णाङ्क y = 0; y < bgr.पङ्क्तिकाः; y += 2)
    {
        ध्रुव अचिह्नित वर्ण* srcY0 = srcY_ + (src_चरणः_Y) * y;
        ध्रुव अचिह्नित वर्ण* srcY1 = srcY0 + src_चरणः_Y;

        ध्रुव अचिह्नित वर्ण* srcV = srcV_ + (src_चरणः_V) * y / 2;
        ध्रुव अचिह्नित वर्ण* srcU = srcU_ + (src_चरणः_U) * y / 2;

        अचिह्नित वर्ण* dst0 = bgr.दत्त + (dst_चरणः_3) * y;
        अचिह्नित वर्ण* dst1 = dst0 + dst_चरणः_3;

        प्रति (पूर्णाङ्क x = 0; x < bgr.स्तम्भसङ्ख्या; x += 2)
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

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<अचिह्नित वर्ण>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<अचिह्नित वर्ण>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<अचिह्नित वर्ण>(Y3 + ruv);
        }
    }
}

स्थिर शून्य copy_convert_bgr_गन्तव्यम्_yv12(ध्रुव VAImage& image, ध्रुव Mat& bgr, अचिह्नित वर्ण* बफर)
{
    ध्रुव भिन्न d1 = 16.0f;
    ध्रुव भिन्न d2 = 128.0f;

    स्थिर ध्रुव भिन्न coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.अङ्कः_planes, (size_t)3, "");

    ध्रुव size_t dst_विस्थापनम्_Y = image.offsets[0];
    ध्रुव size_t dst_विस्थापनम्_V = image.offsets[1];
    ध्रुव size_t dst_विस्थापनम्_U = image.offsets[2];

    ध्रुव size_t dst_चरणः_Y = image.pitches[0];
    ध्रुव size_t dst_चरणः_U = image.pitches[1];
    ध्रुव size_t dst_चरणः_V = image.pitches[2];

    अचिह्नित वर्ण* dstY_ = बफर + dst_विस्थापनम्_Y;
    अचिह्नित वर्ण* dstV_ = बफर + dst_विस्थापनम्_V;
    अचिह्नित वर्ण* dstU_ = बफर + dst_विस्थापनम्_U;

    ध्रुव size_t src_चरणः_3 = bgr.चरणः;

    प्रति (पूर्णाङ्क y = 0; y < bgr.पङ्क्तिकाः; y += 2)
    {
        अचिह्नित वर्ण* dstY0 = dstY_ + (dst_चरणः_Y) * y;
        अचिह्नित वर्ण* dstY1 = dstY0 + dst_चरणः_Y;

        अचिह्नित वर्ण* dstV = dstV_ + (dst_चरणः_V) * y / 2;
        अचिह्नित वर्ण* dstU = dstU_ + (dst_चरणः_U) * y / 2;

        ध्रुव अचिह्नित वर्ण* src0 = bgr.दत्त + (src_चरणः_3) * y;
        ध्रुव अचिह्नित वर्ण* src1 = src0 + src_चरणः_3;

        प्रति (पूर्णाङ्क x = 0; x < bgr.स्तम्भसङ्ख्या; x += 2)
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

            dstY0[x+0] = saturate_cast<अचिह्नित वर्ण>(Y0);
            dstY0[x+1] = saturate_cast<अचिह्नित वर्ण>(Y1);
            dstY1[x+0] = saturate_cast<अचिह्नित वर्ण>(Y2);
            dstY1[x+1] = saturate_cast<अचिह्नित वर्ण>(Y3);

            dstU[x/2] = saturate_cast<अचिह्नित वर्ण>(U);
            dstV[x/2] = saturate_cast<अचिह्नित वर्ण>(V);
        }
    }
}
#यदि_अन्तः // HAVE_VA

शून्य convert_गन्तव्यम्_VASurface(VADisplay display, निवेशः_Array src, VASurfaceID surface, आकार size)
{
    CV_अप्रयुक्तम्(display); CV_अप्रयुक्तम्(src); CV_अप्रयुक्तम्(surface); CV_अप्रयुक्तम्(size);
#यदि !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

    ध्रुव पूर्णाङ्क stype = CV_8UC3;

    पूर्णाङ्क srcType = src.type();
    CV_Assert(srcType == stype);

    आकार src_आकार = src.size();
    CV_Assert(src_आकार.विस्तारः == size.विस्तारः && src_आकार.height == size.height);

#यदि_परिभाषितम् HAVE_VA_INTEL
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

        cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int स्थितिः = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &स्थितिः);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &स्थितिः);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        स्थितिः = interop->cl_पङ्क्तौ_निक्षिप_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        यदि (!ocl::ocl_convert_bgr_गन्तव्यम्_nv12(cl_बफर, (पूर्णाङ्क)u.चरणः[0], u.स्तम्भसङ्ख्या, u.पङ्क्तिकाः, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->cl_पङ्क्तौ_निक्षिप_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        स्थितिः = clFinish(q); // TODO Use events
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        स्थितिः = clReleaseMemObject(clImageY); // TODO RAII
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        स्थितिः = clReleaseMemObject(clImageUV);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    अन्यथा
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.दत्त == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus स्थितिः = 0;

        स्थितिः = vaSyncSurface(display, surface);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        तार्किक परोक्षम्_बफर = असत्य;
        VAImage image;
        स्थितिः = vaDeriveImage(display, surface, &image);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            परोक्षम्_बफर = सत्य;
            पूर्णाङ्क अङ्कः_formats = va_Max_अङ्कः_Image_Formats(display);
            यदि (अङ्कः_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(अङ्कः_formats);

            स्थितिः = vaQueryImageFormats(display, fmt_list.दत्त(), &अङ्कः_formats);
            यदि (स्थितिः != VA_स्थितिः_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = शून्यसूचकः;
            प्रति (स्वचालित &fmt : fmt_list){
                यदि (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    भङ्ग;
                }
            }
            यदि (selected_format == शून्यसूचकः)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            स्थितिः = vaCreateImage(display, selected_format, size.विस्तारः, size.height, &image);
            यदि (स्थितिः != VA_स्थितिः_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        अचिह्नित वर्ण* बफर = 0;
        स्थितिः = va_Map_बफर(display, image.buf, (शून्य **)&बफर);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        यदि (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_गन्तव्यम्_nv12(image, m, बफर);
        यदि (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_गन्तव्यम्_yv12(image, m, बफर);
        अन्यथा
            CV_Check((पूर्णाङ्क)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        स्थितिः = va_Unmap_बफर(display, image.buf);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        यदि (परोक्षम्_बफर){
            स्थितिः = vaPutImage(display, surface, image.image_id, 0, 0, size.विस्तारः, size.height, 0, 0, size.विस्तारः, size.height);
            यदि (स्थितिः != VA_स्थितिः_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        स्थितिः = vaDestroyImage(display, image.image_id);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#यदि_अन्तः  // !HAVE_VA
}

शून्य convertFromVASurface(VADisplay display, VASurfaceID surface, आकार size, OutputArray dst)
{
    CV_अप्रयुक्तम्(display); CV_अप्रयुक्तम्(surface); CV_अप्रयुक्तम्(dst); CV_अप्रयुक्तम्(size);
#यदि !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

    ध्रुव पूर्णाङ्क dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#यदि_परिभाषितम् HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    यदि (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int स्थितिः = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &स्थितिः);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &स्थितिः);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        स्थितिः = interop->cl_पङ्क्तौ_निक्षिप_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        यदि (!ocl::ocl_convert_nv12_गन्तव्यम्_bgr(clImageY, clImageUV, cl_बफर, (पूर्णाङ्क)u.चरणः[0], u.स्तम्भसङ्ख्या, u.पङ्क्तिकाः))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        स्थितिः = interop->cl_पङ्क्तौ_निक्षिप_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, रिक्त, रिक्त);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        स्थितिः = clFinish(q); // TODO Use events
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        स्थितिः = clReleaseMemObject(clImageY); // TODO RAII
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        स्थितिः = clReleaseMemObject(clImageUV);
        यदि (स्थितिः != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    अन्यथा
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.दत्त == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus स्थितिः = 0;

        स्थितिः = vaSyncSurface(display, surface);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        स्थितिः = vaDeriveImage(display, surface, &image);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            पूर्णाङ्क अङ्कः_formats = va_Max_अङ्कः_Image_Formats(display);
            यदि (अङ्कः_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(अङ्कः_formats);

            स्थितिः = vaQueryImageFormats(display, fmt_list.दत्त(), &अङ्कः_formats);
            यदि (स्थितिः != VA_स्थितिः_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = शून्यसूचकः;
            प्रति (स्वचालित &fmt : fmt_list){
                यदि (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    भङ्ग;
                }
            }
            यदि (selected_format == शून्यसूचकः)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            स्थितिः = vaCreateImage(display, selected_format, size.विस्तारः, size.height, &image);
            यदि (स्थितिः != VA_स्थितिः_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            स्थितिः = vaGetImage(display, surface, 0, 0, size.विस्तारः, size.height, image.image_id);
            यदि (स्थितिः != VA_स्थितिः_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        अचिह्नित वर्ण* बफर = 0;
        स्थितिः = va_Map_बफर(display, image.buf, (शून्य **)&बफर);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        यदि (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_गन्तव्यम्_bgr(image, बफर, m);
        यदि (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_गन्तव्यम्_bgr(image, बफर, m);
        अन्यथा
            CV_Check((पूर्णाङ्क)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        स्थितिः = va_Unmap_बफर(display, image.buf);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        स्थितिः = vaDestroyImage(display, image.image_id);
        यदि (स्थितिः != VA_स्थितिः_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#यदि_अन्तः  // !HAVE_VA
}

}} // namespace cv::va_intel
