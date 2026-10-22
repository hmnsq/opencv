// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#समावेश "precomp.hpp"

#समावेश <opencv2/core/utils/logger.hpp>

#अगर_घोषित HAVE_VA
#  include <va/va.h>
#अन्यथा  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#पूर्ण_अगर // HAVE_VA

using namespace cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#अगर_घोषित HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#पूर्ण_अगर // HAVE_OPENCL

#अगर_घोषित HAVE_VA_INTEL
#अगर_घोषित HAVE_VA_INTEL_OLD_HEADER
#  include <CL/va_ext.h>
#अन्यथा
#  include <CL/cl_va_api_media_sharing_intel.h>
#पूर्ण_अगर
#पूर्ण_अगर

#अगर_घोषित HAVE_VA
#अगर_अघोषित OPENCV_LIBVA_LINK
#समावेश "va_wrapper.impl.hpp"
#अन्यथा
namespace cv { namespace detail {
अटल व्योम init_libva() { /* nothing */ }
}}  // namespace
#पूर्ण_अगर
using namespace cv::detail;
#पूर्ण_अगर

namespace cv { namespace va_intel {

#अगर_घोषित HAVE_VA_INTEL

श्रेणी VAAPIInterop : खुला ocl::Context::UserContext
{
खुला:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        अगर (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    भव ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#पूर्ण_अगर // HAVE_VA_INTEL

namespace ocl {

Context& initializeContextFromVA(VADisplay display, bool tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#अगर !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    अगर (tryInterop)
    {
        cl_uint अंक_Platforms;
        cl_int status = clGetPlatformIDs(0, शून्य, &अंक_Platforms);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        अगर (अंक_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(अंक_Platforms);
        status = clGetPlatformIDs(अंक_Platforms, &platforms[0], शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        पूर्णांक found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        क्रम (पूर्णांक i = 0; i < (पूर्णांक)अंक_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            अगर ((व्योम*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == शून्य)
            {
                जारी;
            }

            // Query device list

            cl_uint अंक_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, शून्य, &अंक_Devices);
            अगर ((status != CL_SUCCESS) || !(अंक_Devices > 0))
                जारी;
            अंक_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, अंक_Devices, &device, शून्य);
            अगर (status != CL_SUCCESS)
                जारी;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, अंक_Devices, &device, शून्य, शून्य, &status);
            अगर (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            अन्यथा
            {
                found = i;
                अवरोध;
            }
        }

        अगर (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            प्रयास
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            पकड़ो (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                फेंको;
            }
            clExecCtx.bind();
            वापस const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        वापस ctx;
    }
#पूर्ण_अगर  // !HAVE_VA
}

#अगर_घोषित HAVE_VA_INTEL
अटल bool ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_बफर, पूर्णांक step, पूर्णांक cols, पूर्णांक rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    अगर (k.empty())
        वापस false;

    k.args(clImageY, clImageUV, cl_बफर, step, cols, rows);

    माप_प्रकार globalsize[] = { (माप_प्रकार)cols/2, (माप_प्रकार)rows/2 };
    वापस k.run(2, globalsize, 0, false);
}

अटल bool ocl_convert_bgr_to_nv12(cl_mem cl_बफर, पूर्णांक step, पूर्णांक cols, पूर्णांक rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    अगर (k.empty())
        वापस false;

    k.args(cl_बफर, step, cols, rows, clImageY, clImageUV);

    माप_प्रकार globalsize[] = { (माप_प्रकार)cols/2, (माप_प्रकार)rows/2 };
    वापस k.run(2, globalsize, 0, false);
}
#पूर्ण_अगर // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#अगर defined(HAVE_VA)
स्थिर पूर्णांक NCHANNELS = 3;

अटल व्योम copy_convert_nv12_to_bgr(स्थिर VAImage& image, स्थिर अचिन्हित अक्षर* बफर, Mat& bgr)
{
    स्थिर भग्न d1 = 16.0f;
    स्थिर भग्न d2 = 128.0f;

    अटल स्थिर भग्न coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    स्थिर माप_प्रकार srcOffsetY = image.offsets[0];
    स्थिर माप_प्रकार srcOffsetUV = image.offsets[1];

    स्थिर माप_प्रकार srcStepY = image.pitches[0];
    स्थिर माप_प्रकार srcStepUV = image.pitches[1];

    स्थिर माप_प्रकार dstStep = bgr.step;

    स्थिर अचिन्हित अक्षर* srcY0 = बफर + srcOffsetY;
    स्थिर अचिन्हित अक्षर* srcUV = बफर + srcOffsetUV;

    अचिन्हित अक्षर* dst0 = bgr.तथ्य;

    क्रम (पूर्णांक y = 0; y < bgr.rows; y += 2)
    {
        स्थिर अचिन्हित अक्षर* srcY1 = srcY0 + srcStepY;
        अचिन्हित अक्षर *dst1 = dst0 + dstStep;

        क्रम (पूर्णांक x = 0; x < bgr.cols; x += 2)
        {
            भग्न Y0 = भग्न(srcY0[x+0]);
            भग्न Y1 = भग्न(srcY0[x+1]);
            भग्न Y2 = भग्न(srcY1[x+0]);
            भग्न Y3 = भग्न(srcY1[x+1]);

            भग्न U = भग्न(srcUV[2*(x/2)+0]) - d2;
            भग्न V = भग्न(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            भग्न ruv = coeffs[4]*V;
            भग्न guv = coeffs[3]*V + coeffs[2]*U;
            भग्न buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

अटल व्योम copy_convert_bgr_to_nv12(स्थिर VAImage& image, स्थिर Mat& bgr, अचिन्हित अक्षर* बफर)
{
    स्थिर भग्न d1 = 16.0f;
    स्थिर भग्न d2 = 128.0f;

    अटल स्थिर भग्न coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    स्थिर माप_प्रकार dstOffsetY = image.offsets[0];
    स्थिर माप_प्रकार dstOffsetUV = image.offsets[1];

    स्थिर माप_प्रकार dstStepY = image.pitches[0];
    स्थिर माप_प्रकार dstStepUV = image.pitches[1];

    स्थिर माप_प्रकार srcStep = bgr.step;

    स्थिर अचिन्हित अक्षर* src0 = bgr.तथ्य;

    अचिन्हित अक्षर* dstY0 = बफर + dstOffsetY;
    अचिन्हित अक्षर* dstUV = बफर + dstOffsetUV;

    क्रम (पूर्णांक y = 0; y < bgr.rows; y += 2)
    {
        स्थिर अचिन्हित अक्षर *src1 = src0 + srcStep;
        अचिन्हित अक्षर* dstY1 = dstY0 + dstStepY;

        क्रम (पूर्णांक x = 0; x < bgr.cols; x += 2)
        {
            भग्न B0 = भग्न(src0[(x+0)*NCHANNELS+0]);
            भग्न G0 = भग्न(src0[(x+0)*NCHANNELS+1]);
            भग्न R0 = भग्न(src0[(x+0)*NCHANNELS+2]);

            भग्न B1 = भग्न(src0[(x+1)*NCHANNELS+0]);
            भग्न G1 = भग्न(src0[(x+1)*NCHANNELS+1]);
            भग्न R1 = भग्न(src0[(x+1)*NCHANNELS+2]);

            भग्न B2 = भग्न(src1[(x+0)*NCHANNELS+0]);
            भग्न G2 = भग्न(src1[(x+0)*NCHANNELS+1]);
            भग्न R2 = भग्न(src1[(x+0)*NCHANNELS+2]);

            भग्न B3 = भग्न(src1[(x+1)*NCHANNELS+0]);
            भग्न G3 = भग्न(src1[(x+1)*NCHANNELS+1]);
            भग्न R3 = भग्न(src1[(x+1)*NCHANNELS+2]);

            भग्न Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            भग्न Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            भग्न Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            भग्न Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            भग्न U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            भग्न V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<अचिन्हित अक्षर>(Y0);
            dstY0[x+1] = saturate_cast<अचिन्हित अक्षर>(Y1);
            dstY1[x+0] = saturate_cast<अचिन्हित अक्षर>(Y2);
            dstY1[x+1] = saturate_cast<अचिन्हित अक्षर>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<अचिन्हित अक्षर>(U);
            dstUV[2*(x/2)+1] = saturate_cast<अचिन्हित अक्षर>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


अटल व्योम copy_convert_yv12_to_bgr(स्थिर VAImage& image, स्थिर अचिन्हित अक्षर* बफर, Mat& bgr)
{
    स्थिर भग्न d1 = 16.0f;
    स्थिर भग्न d2 = 128.0f;

    अटल स्थिर भग्न coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((माप_प्रकार)image.format.fourcc, (माप_प्रकार)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((माप_प्रकार)image.अंक_planes, (माप_प्रकार)3, "");

    स्थिर माप_प्रकार srcOffsetY = image.offsets[0];
    स्थिर माप_प्रकार srcOffsetV = image.offsets[1];
    स्थिर माप_प्रकार srcOffsetU = image.offsets[2];

    स्थिर माप_प्रकार srcStepY = image.pitches[0];
    स्थिर माप_प्रकार srcStepU = image.pitches[1];
    स्थिर माप_प्रकार srcStepV = image.pitches[2];

    स्थिर माप_प्रकार dstStep = bgr.step;

    स्थिर अचिन्हित अक्षर* srcY_ = बफर + srcOffsetY;
    स्थिर अचिन्हित अक्षर* srcV_ = बफर + srcOffsetV;
    स्थिर अचिन्हित अक्षर* srcU_ = बफर + srcOffsetU;

    क्रम (पूर्णांक y = 0; y < bgr.rows; y += 2)
    {
        स्थिर अचिन्हित अक्षर* srcY0 = srcY_ + (srcStepY) * y;
        स्थिर अचिन्हित अक्षर* srcY1 = srcY0 + srcStepY;

        स्थिर अचिन्हित अक्षर* srcV = srcV_ + (srcStepV) * y / 2;
        स्थिर अचिन्हित अक्षर* srcU = srcU_ + (srcStepU) * y / 2;

        अचिन्हित अक्षर* dst0 = bgr.तथ्य + (dstStep) * y;
        अचिन्हित अक्षर* dst1 = dst0 + dstStep;

        क्रम (पूर्णांक x = 0; x < bgr.cols; x += 2)
        {
            भग्न Y0 = भग्न(srcY0[x+0]);
            भग्न Y1 = भग्न(srcY0[x+1]);
            भग्न Y2 = भग्न(srcY1[x+0]);
            भग्न Y3 = भग्न(srcY1[x+1]);

            भग्न U = भग्न(srcU[x/2]) - d2;
            भग्न V = भग्न(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            भग्न ruv = coeffs[4]*V;
            भग्न guv = coeffs[3]*V + coeffs[2]*U;
            भग्न buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<अचिन्हित अक्षर>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<अचिन्हित अक्षर>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<अचिन्हित अक्षर>(Y3 + ruv);
        }
    }
}

अटल व्योम copy_convert_bgr_to_yv12(स्थिर VAImage& image, स्थिर Mat& bgr, अचिन्हित अक्षर* बफर)
{
    स्थिर भग्न d1 = 16.0f;
    स्थिर भग्न d2 = 128.0f;

    अटल स्थिर भग्न coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((माप_प्रकार)image.format.fourcc, (माप_प्रकार)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((माप_प्रकार)image.अंक_planes, (माप_प्रकार)3, "");

    स्थिर माप_प्रकार dstOffsetY = image.offsets[0];
    स्थिर माप_प्रकार dstOffsetV = image.offsets[1];
    स्थिर माप_प्रकार dstOffsetU = image.offsets[2];

    स्थिर माप_प्रकार dstStepY = image.pitches[0];
    स्थिर माप_प्रकार dstStepU = image.pitches[1];
    स्थिर माप_प्रकार dstStepV = image.pitches[2];

    अचिन्हित अक्षर* dstY_ = बफर + dstOffsetY;
    अचिन्हित अक्षर* dstV_ = बफर + dstOffsetV;
    अचिन्हित अक्षर* dstU_ = बफर + dstOffsetU;

    स्थिर माप_प्रकार srcStep = bgr.step;

    क्रम (पूर्णांक y = 0; y < bgr.rows; y += 2)
    {
        अचिन्हित अक्षर* dstY0 = dstY_ + (dstStepY) * y;
        अचिन्हित अक्षर* dstY1 = dstY0 + dstStepY;

        अचिन्हित अक्षर* dstV = dstV_ + (dstStepV) * y / 2;
        अचिन्हित अक्षर* dstU = dstU_ + (dstStepU) * y / 2;

        स्थिर अचिन्हित अक्षर* src0 = bgr.तथ्य + (srcStep) * y;
        स्थिर अचिन्हित अक्षर* src1 = src0 + srcStep;

        क्रम (पूर्णांक x = 0; x < bgr.cols; x += 2)
        {
            भग्न B0 = भग्न(src0[(x+0)*NCHANNELS+0]);
            भग्न G0 = भग्न(src0[(x+0)*NCHANNELS+1]);
            भग्न R0 = भग्न(src0[(x+0)*NCHANNELS+2]);

            भग्न B1 = भग्न(src0[(x+1)*NCHANNELS+0]);
            भग्न G1 = भग्न(src0[(x+1)*NCHANNELS+1]);
            भग्न R1 = भग्न(src0[(x+1)*NCHANNELS+2]);

            भग्न B2 = भग्न(src1[(x+0)*NCHANNELS+0]);
            भग्न G2 = भग्न(src1[(x+0)*NCHANNELS+1]);
            भग्न R2 = भग्न(src1[(x+0)*NCHANNELS+2]);

            भग्न B3 = भग्न(src1[(x+1)*NCHANNELS+0]);
            भग्न G3 = भग्न(src1[(x+1)*NCHANNELS+1]);
            भग्न R3 = भग्न(src1[(x+1)*NCHANNELS+2]);

            भग्न Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            भग्न Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            भग्न Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            भग्न Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            भग्न U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            भग्न V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<अचिन्हित अक्षर>(Y0);
            dstY0[x+1] = saturate_cast<अचिन्हित अक्षर>(Y1);
            dstY1[x+0] = saturate_cast<अचिन्हित अक्षर>(Y2);
            dstY1[x+1] = saturate_cast<अचिन्हित अक्षर>(Y3);

            dstU[x/2] = saturate_cast<अचिन्हित अक्षर>(U);
            dstV[x/2] = saturate_cast<अचिन्हित अक्षर>(V);
        }
    }
}
#पूर्ण_अगर // HAVE_VA

व्योम convertToVASurface(VADisplay display, निवेश_Array src, VASurfaceID surface, आकार size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#अगर !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

    स्थिर पूर्णांक stype = CV_8UC3;

    पूर्णांक srcType = src.type();
    CV_Assert(srcType == stype);

    आकार src_आकार = src.size();
    CV_Assert(src_आकार.width == size.width && src_आकार.height == size.height);

#अगर_घोषित HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().लो();
    CV_LOG_IF_DEBUG(शून्य, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    व्योम* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(शून्य, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    bool isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(शून्य, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (व्योम*)display << "(surface)");
    अगर (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        अगर (!ocl::ocl_convert_bgr_to_nv12(cl_बफर, (पूर्णांक)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    अन्यथा
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.तथ्य == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        bool indirect_बफर = false;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        अगर (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_बफर = true;
            पूर्णांक अंक_formats = va_Max_अंक_Image_Formats(display);
            अगर (अंक_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(अंक_formats);

            status = vaQueryImageFormats(display, fmt_list.तथ्य(), &अंक_formats);
            अगर (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            क्रम (स्वतः &fmt : fmt_list){
                अगर (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    अवरोध;
                }
            }
            अगर (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            अगर (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        अचिन्हित अक्षर* बफर = 0;
        status = va_Map_बफर(display, image.buf, (व्योम **)&बफर);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        अगर (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, बफर);
        अगर (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, बफर);
        अन्यथा
            CV_Check((पूर्णांक)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = va_Unmap_बफर(display, image.buf);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        अगर (indirect_बफर){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            अगर (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#पूर्ण_अगर  // !HAVE_VA
}

व्योम convertFromVASurface(VADisplay display, VASurfaceID surface, आकार size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#अगर !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#अन्यथा  // !HAVE_VA

    स्थिर पूर्णांक dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#अगर_घोषित HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().लो();
    अगर (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_बफर = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        अगर (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, cl_बफर, (पूर्णांक)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, शून्य, शून्य);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        अगर (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    अन्यथा
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.तथ्य == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        अगर (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            पूर्णांक अंक_formats = va_Max_अंक_Image_Formats(display);
            अगर (अंक_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(अंक_formats);

            status = vaQueryImageFormats(display, fmt_list.तथ्य(), &अंक_formats);
            अगर (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            क्रम (स्वतः &fmt : fmt_list){
                अगर (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    अवरोध;
                }
            }
            अगर (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            अगर (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            अगर (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        अचिन्हित अक्षर* बफर = 0;
        status = va_Map_बफर(display, image.buf, (व्योम **)&बफर);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        अगर (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, बफर, m);
        अगर (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, बफर, m);
        अन्यथा
            CV_Check((पूर्णांक)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = va_Unmap_बफर(display, image.buf);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        अगर (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#पूर्ण_अगर  // !HAVE_VA
}

}} // namespace cv::va_intel
