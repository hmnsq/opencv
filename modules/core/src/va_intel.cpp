// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#include "precomp.hpp"

#include <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#དེ་མིན  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using namespace cv;

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
#དེ་མིན
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#include "va_wrapper.impl.hpp"
#དེ་མིན
namespace cv { namespace detail {
static void init_libva() { /* nothing */ }
}}  // namespace
#endif
using namespace cv::detail;
#endif

namespace cv { namespace va_intel {

#ifdef HAVE_VA_INTEL

class VAAPIInterop : public ocl::Context::UserContext
{
public:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        གལ་ཏེ (!clCreateFromVA_APIMediaSurfaceINTEL ||
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

namespace ocl {

Context& initializeContextFromVA(VADisplay display, bool tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#གལ་ཏེ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#དེ་མིན  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    གལ་ཏེ (tryInterop)
    {
        cl_uint numPlatforms;
        cl_int status = clGetPlatformIDs(0, NULL, &numPlatforms);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        གལ་ཏེ (numPlatforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(numPlatforms);
        status = clGetPlatformIDs(numPlatforms, &platforms[0], NULL);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        ཧྲིལ found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        ཆེད་དུ (ཧྲིལ i = 0; i < (ཧྲིལ)numPlatforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            གལ་ཏེ ((void*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == NULL)
            {
                continue;
            }

            // Query device list

            cl_uint numDevices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, NULL, &numDevices);
            གལ་ཏེ ((status != CL_SUCCESS) || !(numDevices > 0))
                continue;
            numDevices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, numDevices, &device, NULL);
            གལ་ཏེ (status != CL_SUCCESS)
                continue;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, numDevices, &device, NULL, NULL, &status);
            གལ་ཏེ (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            དེ་མིན
            {
                found = i;
                bha_nga;
            }
        }

        གལ་ཏེ (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            try
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            gr_haa_na (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                k_sipa;
            }
            clExecCtx.bind();
            ཕྱིར const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        ཕྱིར ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
static bool ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, ཧྲིལ step, ཧྲིལ cols, ཧྲིལ rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    གལ་ཏེ (k.empty())
        ཕྱིར false;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ཕྱིར k.run(2, globalsize, 0, false);
}

static bool ocl_convert_bgr_to_nv12(cl_mem clBuffer, ཧྲིལ step, ཧྲིལ cols, ཧྲིལ rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    གལ་ཏེ (k.empty())
        ཕྱིར false;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    ཕྱིར k.run(2, globalsize, 0, false);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#གལ་ཏེ defined(HAVE_VA)
const ཧྲིལ NCHANNELS = 3;

static void copy_convert_nv12_to_bgr(const VAImage& image, const unsigned var_na* buffer, Mat& bgr)
{
    const float d1 = 16.0f;
    const float d2 = 128.0f;

    static const float coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    const size_t srcOffsetY = image.offsets[0];
    const size_t srcOffsetUV = image.offsets[1];

    const size_t srcStepY = image.pitches[0];
    const size_t srcStepUV = image.pitches[1];

    const size_t dstStep = bgr.step;

    const unsigned var_na* srcY0 = buffer + srcOffsetY;
    const unsigned var_na* srcUV = buffer + srcOffsetUV;

    unsigned var_na* dst0 = bgr.data;

    ཆེད་དུ (ཧྲིལ y = 0; y < bgr.rows; y += 2)
    {
        const unsigned var_na* srcY1 = srcY0 + srcStepY;
        unsigned var_na *dst1 = dst0 + dstStep;

        ཆེད་དུ (ཧྲིལ x = 0; x < bgr.cols; x += 2)
        {
            float Y0 = float(srcY0[x+0]);
            float Y1 = float(srcY0[x+1]);
            float Y2 = float(srcY1[x+0]);
            float Y3 = float(srcY1[x+1]);

            float U = float(srcUV[2*(x/2)+0]) - d2;
            float V = float(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            float ruv = coeffs[4]*V;
            float guv = coeffs[3]*V + coeffs[2]*U;
            float buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

static void copy_convert_bgr_to_nv12(const VAImage& image, const Mat& bgr, unsigned var_na* buffer)
{
    const float d1 = 16.0f;
    const float d2 = 128.0f;

    static const float coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    const size_t dstOffsetY = image.offsets[0];
    const size_t dstOffsetUV = image.offsets[1];

    const size_t dstStepY = image.pitches[0];
    const size_t dstStepUV = image.pitches[1];

    const size_t srcStep = bgr.step;

    const unsigned var_na* src0 = bgr.data;

    unsigned var_na* dstY0 = buffer + dstOffsetY;
    unsigned var_na* dstUV = buffer + dstOffsetUV;

    ཆེད་དུ (ཧྲིལ y = 0; y < bgr.rows; y += 2)
    {
        const unsigned var_na *src1 = src0 + srcStep;
        unsigned var_na* dstY1 = dstY0 + dstStepY;

        ཆེད་དུ (ཧྲིལ x = 0; x < bgr.cols; x += 2)
        {
            float B0 = float(src0[(x+0)*NCHANNELS+0]);
            float G0 = float(src0[(x+0)*NCHANNELS+1]);
            float R0 = float(src0[(x+0)*NCHANNELS+2]);

            float B1 = float(src0[(x+1)*NCHANNELS+0]);
            float G1 = float(src0[(x+1)*NCHANNELS+1]);
            float R1 = float(src0[(x+1)*NCHANNELS+2]);

            float B2 = float(src1[(x+0)*NCHANNELS+0]);
            float G2 = float(src1[(x+0)*NCHANNELS+1]);
            float R2 = float(src1[(x+0)*NCHANNELS+2]);

            float B3 = float(src1[(x+1)*NCHANNELS+0]);
            float G3 = float(src1[(x+1)*NCHANNELS+1]);
            float R3 = float(src1[(x+1)*NCHANNELS+2]);

            float Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            float Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            float Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            float Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            float U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            float V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned var_na>(Y0);
            dstY0[x+1] = saturate_cast<unsigned var_na>(Y1);
            dstY1[x+0] = saturate_cast<unsigned var_na>(Y2);
            dstY1[x+1] = saturate_cast<unsigned var_na>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned var_na>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned var_na>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


static void copy_convert_yv12_to_bgr(const VAImage& image, const unsigned var_na* buffer, Mat& bgr)
{
    const float d1 = 16.0f;
    const float d2 = 128.0f;

    static const float coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.num_planes, (size_t)3, "");

    const size_t srcOffsetY = image.offsets[0];
    const size_t srcOffsetV = image.offsets[1];
    const size_t srcOffsetU = image.offsets[2];

    const size_t srcStepY = image.pitches[0];
    const size_t srcStepU = image.pitches[1];
    const size_t srcStepV = image.pitches[2];

    const size_t dstStep = bgr.step;

    const unsigned var_na* srcY_ = buffer + srcOffsetY;
    const unsigned var_na* srcV_ = buffer + srcOffsetV;
    const unsigned var_na* srcU_ = buffer + srcOffsetU;

    ཆེད་དུ (ཧྲིལ y = 0; y < bgr.rows; y += 2)
    {
        const unsigned var_na* srcY0 = srcY_ + (srcStepY) * y;
        const unsigned var_na* srcY1 = srcY0 + srcStepY;

        const unsigned var_na* srcV = srcV_ + (srcStepV) * y / 2;
        const unsigned var_na* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned var_na* dst0 = bgr.data + (dstStep) * y;
        unsigned var_na* dst1 = dst0 + dstStep;

        ཆེད་དུ (ཧྲིལ x = 0; x < bgr.cols; x += 2)
        {
            float Y0 = float(srcY0[x+0]);
            float Y1 = float(srcY0[x+1]);
            float Y2 = float(srcY1[x+0]);
            float Y3 = float(srcY1[x+1]);

            float U = float(srcU[x/2]) - d2;
            float V = float(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            float ruv = coeffs[4]*V;
            float guv = coeffs[3]*V + coeffs[2]*U;
            float buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned var_na>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned var_na>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned var_na>(Y3 + ruv);
        }
    }
}

static void copy_convert_bgr_to_yv12(const VAImage& image, const Mat& bgr, unsigned var_na* buffer)
{
    const float d1 = 16.0f;
    const float d2 = 128.0f;

    static const float coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.num_planes, (size_t)3, "");

    const size_t dstOffsetY = image.offsets[0];
    const size_t dstOffsetV = image.offsets[1];
    const size_t dstOffsetU = image.offsets[2];

    const size_t dstStepY = image.pitches[0];
    const size_t dstStepU = image.pitches[1];
    const size_t dstStepV = image.pitches[2];

    unsigned var_na* dstY_ = buffer + dstOffsetY;
    unsigned var_na* dstV_ = buffer + dstOffsetV;
    unsigned var_na* dstU_ = buffer + dstOffsetU;

    const size_t srcStep = bgr.step;

    ཆེད་དུ (ཧྲིལ y = 0; y < bgr.rows; y += 2)
    {
        unsigned var_na* dstY0 = dstY_ + (dstStepY) * y;
        unsigned var_na* dstY1 = dstY0 + dstStepY;

        unsigned var_na* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned var_na* dstU = dstU_ + (dstStepU) * y / 2;

        const unsigned var_na* src0 = bgr.data + (srcStep) * y;
        const unsigned var_na* src1 = src0 + srcStep;

        ཆེད་དུ (ཧྲིལ x = 0; x < bgr.cols; x += 2)
        {
            float B0 = float(src0[(x+0)*NCHANNELS+0]);
            float G0 = float(src0[(x+0)*NCHANNELS+1]);
            float R0 = float(src0[(x+0)*NCHANNELS+2]);

            float B1 = float(src0[(x+1)*NCHANNELS+0]);
            float G1 = float(src0[(x+1)*NCHANNELS+1]);
            float R1 = float(src0[(x+1)*NCHANNELS+2]);

            float B2 = float(src1[(x+0)*NCHANNELS+0]);
            float G2 = float(src1[(x+0)*NCHANNELS+1]);
            float R2 = float(src1[(x+0)*NCHANNELS+2]);

            float B3 = float(src1[(x+1)*NCHANNELS+0]);
            float G3 = float(src1[(x+1)*NCHANNELS+1]);
            float R3 = float(src1[(x+1)*NCHANNELS+2]);

            float Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            float Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            float Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            float Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            float U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            float V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned var_na>(Y0);
            dstY0[x+1] = saturate_cast<unsigned var_na>(Y1);
            dstY1[x+0] = saturate_cast<unsigned var_na>(Y2);
            dstY1[x+1] = saturate_cast<unsigned var_na>(Y3);

            dstU[x/2] = saturate_cast<unsigned var_na>(U);
            dstV[x/2] = saturate_cast<unsigned var_na>(V);
        }
    }
}
#endif // HAVE_VA

void convertToVASurface(VADisplay display, InputArray src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#གལ་ཏེ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#དེ་མིན  // !HAVE_VA

    const ཧྲིལ stype = CV_8UC3;

    ཧྲིལ srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(NULL, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    void* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(NULL, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    bool isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(NULL, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (void*)display << "(surface)");
    གལ་ཏེ (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, NULL, NULL);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        གལ་ཏེ (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (ཧྲིལ)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, NULL, NULL);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    དེ་མིན
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        bool indirect_buffer = false;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        གལ་ཏེ (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = true;
            ཧྲིལ num_formats = vaMaxNumImageFormats(display);
            གལ་ཏེ (num_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(num_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &num_formats);
            གལ་ཏེ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            ཆེད་དུ (auto &fmt : fmt_list){
                གལ་ཏེ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    bha_nga;
                }
            }
            གལ་ཏེ (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            གལ་ཏེ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned var_na* buffer = 0;
        status = vaMapBuffer(display, image.buf, (void **)&buffer);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        གལ་ཏེ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        གལ་ཏེ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        དེ་མིན
            CV_Check((ཧྲིལ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        གལ་ཏེ (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            གལ་ཏེ (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

void convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#གལ་ཏེ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#དེ་མིན  // !HAVE_VA

    const ཧྲིལ dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    གལ་ཏེ (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, NULL, NULL);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        གལ་ཏེ (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (ཧྲིལ)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, NULL, NULL);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        གལ་ཏེ (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    དེ་མིན
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        གལ་ཏེ (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            ཧྲིལ num_formats = vaMaxNumImageFormats(display);
            གལ་ཏེ (num_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(num_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &num_formats);
            གལ་ཏེ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            ཆེད་དུ (auto &fmt : fmt_list){
                གལ་ཏེ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    bha_nga;
                }
            }
            གལ་ཏེ (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            གལ་ཏེ (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            གལ་ཏེ (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned var_na* buffer = 0;
        status = vaMapBuffer(display, image.buf, (void **)&buffer);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        གལ་ཏེ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        གལ་ཏེ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        དེ་མིན
            CV_Check((ཧྲིལ)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        གལ་ཏེ (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
