// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#અન્તર્ભાવ "precomp.hpp"

#અન્તર્ભાવ <opencv2/core/utils/logger.hpp>

#ifdef HAVE_VA
#  include <va/va.h>
#નહીંતર  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#endif // HAVE_VA

using નામાકાશ cv;

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
#નહીંતર
#  include <CL/cl_va_api_media_sharing_intel.h>
#endif
#endif

#ifdef HAVE_VA
#ifndef OPENCV_LIBVA_LINK
#અન્તર્ભાવ "va_wrapper.impl.hpp"
#નહીંતર
નામાકાશ cv { નામાકાશ detail {
સ્થિર શૂન્ય init_libva() { /* nothing */ }
}}  // namespace
#endif
using નામાકાશ cv::detail;
#endif

નામાકાશ cv { નામાકાશ va_intel {

#ifdef HAVE_VA_INTEL

વર્ગ VAAPIInterop : જાહેર ocl::Context::UserContext
{
જાહેર:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        clEnqueueAcquireVA_APIMediaSurfacesINTEL  = (clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        clEnqueueReleaseVA_APIMediaSurfacesINTEL  = (clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        જો (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !clEnqueueAcquireVA_APIMediaSurfacesINTEL ||
            !clEnqueueReleaseVA_APIMediaSurfacesINTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    આભાસી ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    clEnqueueAcquireVA_APIMediaSurfacesINTEL_fn  clEnqueueAcquireVA_APIMediaSurfacesINTEL;
    clEnqueueReleaseVA_APIMediaSurfacesINTEL_fn  clEnqueueReleaseVA_APIMediaSurfacesINTEL;
};

#endif // HAVE_VA_INTEL

નામાકાશ ocl {

Context& initializeContextFromVA(VADisplay display, તાર્કિક tryInterop)
{
    CV_UNUSED(display); CV_UNUSED(tryInterop);
#જો !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#નહીંતર  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    જો (tryInterop)
    {
        cl_uint અંક_Platforms;
        cl_int status = clGetPlatformIDs(0, ખાલી, &અંક_Platforms);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        જો (અંક_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(અંક_Platforms);
        status = clGetPlatformIDs(અંક_Platforms, &platforms[0], ખાલી);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        પૂર્ણાંક found = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        માટે (પૂર્ણાંક i = 0; i < (પૂર્ણાંક)અંક_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                clGetExtensionFunctionAddressForPlatform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            જો ((શૂન્ય*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ખાલી)
            {
                ચાલુ;
            }

            // Query device list

            cl_uint અંક_Devices = 0;

            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ખાલી, &અંક_Devices);
            જો ((status != CL_SUCCESS) || !(અંક_Devices > 0))
                ચાલુ;
            અંક_Devices = 1; // OpenCV expects only 1 device
            status = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, અંક_Devices, &device, ખાલી);
            જો (status != CL_SUCCESS)
                ચાલુ;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, અંક_Devices, &device, ખાલી, ખાલી, &status);
            જો (status != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            નહીંતર
            {
                found = i;
                તોડો;
            }
        }

        જો (found >= 0)
        {
            cl_platform_id platform = platforms[found];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            પ્રયાસ
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().setUserContext(std::make_shared<VAAPIInterop>(platform));
            }
            પકડો (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                ફેંકો;
            }
            clExecCtx.bind();
            પાછા const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        પાછા ctx;
    }
#endif  // !HAVE_VA
}

#ifdef HAVE_VA_INTEL
સ્થિર તાર્કિક ocl_convert_nv12_to_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem clBuffer, પૂર્ણાંક step, પૂર્ણાંક cols, પૂર્ણાંક rows)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    જો (k.empty())
        પાછા ખોટું;

    k.args(clImageY, clImageUV, clBuffer, step, cols, rows);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    પાછા k.run(2, globalsize, 0, ખોટું);
}

સ્થિર તાર્કિક ocl_convert_bgr_to_nv12(cl_mem clBuffer, પૂર્ણાંક step, પૂર્ણાંક cols, પૂર્ણાંક rows, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    જો (k.empty())
        પાછા ખોટું;

    k.args(clBuffer, step, cols, rows, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)cols/2, (size_t)rows/2 };
    પાછા k.run(2, globalsize, 0, ખોટું);
}
#endif // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#જો defined(HAVE_VA)
અચલ પૂર્ણાંક NCHANNELS = 3;

સ્થિર શૂન્ય copy_convert_nv12_to_bgr(અચલ VAImage& image, અચલ unsigned અક્ષર* buffer, Mat& bgr)
{
    અચલ દશાંશ d1 = 16.0f;
    અચલ દશાંશ d2 = 128.0f;

    સ્થિર અચલ દશાંશ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    અચલ size_t srcOffsetY = image.offsets[0];
    અચલ size_t srcOffsetUV = image.offsets[1];

    અચલ size_t srcStepY = image.pitches[0];
    અચલ size_t srcStepUV = image.pitches[1];

    અચલ size_t dstStep = bgr.step;

    અચલ unsigned અક્ષર* srcY0 = buffer + srcOffsetY;
    અચલ unsigned અક્ષર* srcUV = buffer + srcOffsetUV;

    unsigned અક્ષર* dst0 = bgr.data;

    માટે (પૂર્ણાંક y = 0; y < bgr.rows; y += 2)
    {
        અચલ unsigned અક્ષર* srcY1 = srcY0 + srcStepY;
        unsigned અક્ષર *dst1 = dst0 + dstStep;

        માટે (પૂર્ણાંક x = 0; x < bgr.cols; x += 2)
        {
            દશાંશ Y0 = દશાંશ(srcY0[x+0]);
            દશાંશ Y1 = દશાંશ(srcY0[x+1]);
            દશાંશ Y2 = દશાંશ(srcY1[x+0]);
            દશાંશ Y3 = દશાંશ(srcY1[x+1]);

            દશાંશ U = દશાંશ(srcUV[2*(x/2)+0]) - d2;
            દશાંશ V = દશાંશ(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            દશાંશ ruv = coeffs[4]*V;
            દશાંશ guv = coeffs[3]*V + coeffs[2]*U;
            દશાંશ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y3 + ruv);
        }

        srcY0 = srcY1 + srcStepY;
        srcUV += srcStepUV;
        dst0 = dst1 + dstStep;
    }
}

સ્થિર શૂન્ય copy_convert_bgr_to_nv12(અચલ VAImage& image, અચલ Mat& bgr, unsigned અક્ષર* buffer)
{
    અચલ દશાંશ d1 = 16.0f;
    અચલ દશાંશ d2 = 128.0f;

    સ્થિર અચલ દશાંશ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    અચલ size_t dstOffsetY = image.offsets[0];
    અચલ size_t dstOffsetUV = image.offsets[1];

    અચલ size_t dstStepY = image.pitches[0];
    અચલ size_t dstStepUV = image.pitches[1];

    અચલ size_t srcStep = bgr.step;

    અચલ unsigned અક્ષર* src0 = bgr.data;

    unsigned અક્ષર* dstY0 = buffer + dstOffsetY;
    unsigned અક્ષર* dstUV = buffer + dstOffsetUV;

    માટે (પૂર્ણાંક y = 0; y < bgr.rows; y += 2)
    {
        અચલ unsigned અક્ષર *src1 = src0 + srcStep;
        unsigned અક્ષર* dstY1 = dstY0 + dstStepY;

        માટે (પૂર્ણાંક x = 0; x < bgr.cols; x += 2)
        {
            દશાંશ B0 = દશાંશ(src0[(x+0)*NCHANNELS+0]);
            દશાંશ G0 = દશાંશ(src0[(x+0)*NCHANNELS+1]);
            દશાંશ R0 = દશાંશ(src0[(x+0)*NCHANNELS+2]);

            દશાંશ B1 = દશાંશ(src0[(x+1)*NCHANNELS+0]);
            દશાંશ G1 = દશાંશ(src0[(x+1)*NCHANNELS+1]);
            દશાંશ R1 = દશાંશ(src0[(x+1)*NCHANNELS+2]);

            દશાંશ B2 = દશાંશ(src1[(x+0)*NCHANNELS+0]);
            દશાંશ G2 = દશાંશ(src1[(x+0)*NCHANNELS+1]);
            દશાંશ R2 = દશાંશ(src1[(x+0)*NCHANNELS+2]);

            દશાંશ B3 = દશાંશ(src1[(x+1)*NCHANNELS+0]);
            દશાંશ G3 = દશાંશ(src1[(x+1)*NCHANNELS+1]);
            દશાંશ R3 = દશાંશ(src1[(x+1)*NCHANNELS+2]);

            દશાંશ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            દશાંશ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            દશાંશ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            દશાંશ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            દશાંશ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            દશાંશ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned અક્ષર>(Y0);
            dstY0[x+1] = saturate_cast<unsigned અક્ષર>(Y1);
            dstY1[x+0] = saturate_cast<unsigned અક્ષર>(Y2);
            dstY1[x+1] = saturate_cast<unsigned અક્ષર>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<unsigned અક્ષર>(U);
            dstUV[2*(x/2)+1] = saturate_cast<unsigned અક્ષર>(V);
        }

        src0 = src1 + srcStep;
        dstY0 = dstY1 + dstStepY;
        dstUV += dstStepUV;
    }
}


સ્થિર શૂન્ય copy_convert_yv12_to_bgr(અચલ VAImage& image, અચલ unsigned અક્ષર* buffer, Mat& bgr)
{
    અચલ દશાંશ d1 = 16.0f;
    અચલ દશાંશ d2 = 128.0f;

    સ્થિર અચલ દશાંશ coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.અંક_planes, (size_t)3, "");

    અચલ size_t srcOffsetY = image.offsets[0];
    અચલ size_t srcOffsetV = image.offsets[1];
    અચલ size_t srcOffsetU = image.offsets[2];

    અચલ size_t srcStepY = image.pitches[0];
    અચલ size_t srcStepU = image.pitches[1];
    અચલ size_t srcStepV = image.pitches[2];

    અચલ size_t dstStep = bgr.step;

    અચલ unsigned અક્ષર* srcY_ = buffer + srcOffsetY;
    અચલ unsigned અક્ષર* srcV_ = buffer + srcOffsetV;
    અચલ unsigned અક્ષર* srcU_ = buffer + srcOffsetU;

    માટે (પૂર્ણાંક y = 0; y < bgr.rows; y += 2)
    {
        અચલ unsigned અક્ષર* srcY0 = srcY_ + (srcStepY) * y;
        અચલ unsigned અક્ષર* srcY1 = srcY0 + srcStepY;

        અચલ unsigned અક્ષર* srcV = srcV_ + (srcStepV) * y / 2;
        અચલ unsigned અક્ષર* srcU = srcU_ + (srcStepU) * y / 2;

        unsigned અક્ષર* dst0 = bgr.data + (dstStep) * y;
        unsigned અક્ષર* dst1 = dst0 + dstStep;

        માટે (પૂર્ણાંક x = 0; x < bgr.cols; x += 2)
        {
            દશાંશ Y0 = દશાંશ(srcY0[x+0]);
            દશાંશ Y1 = દશાંશ(srcY0[x+1]);
            દશાંશ Y2 = દશાંશ(srcY1[x+0]);
            દશાંશ Y3 = દશાંશ(srcY1[x+1]);

            દશાંશ U = દશાંશ(srcU[x/2]) - d2;
            દશાંશ V = દશાંશ(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            દશાંશ ruv = coeffs[4]*V;
            દશાંશ guv = coeffs[3]*V + coeffs[2]*U;
            દશાંશ buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<unsigned અક્ષર>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<unsigned અક્ષર>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<unsigned અક્ષર>(Y3 + ruv);
        }
    }
}

સ્થિર શૂન્ય copy_convert_bgr_to_yv12(અચલ VAImage& image, અચલ Mat& bgr, unsigned અક્ષર* buffer)
{
    અચલ દશાંશ d1 = 16.0f;
    અચલ દશાંશ d2 = 128.0f;

    સ્થિર અચલ દશાંશ coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.અંક_planes, (size_t)3, "");

    અચલ size_t dstOffsetY = image.offsets[0];
    અચલ size_t dstOffsetV = image.offsets[1];
    અચલ size_t dstOffsetU = image.offsets[2];

    અચલ size_t dstStepY = image.pitches[0];
    અચલ size_t dstStepU = image.pitches[1];
    અચલ size_t dstStepV = image.pitches[2];

    unsigned અક્ષર* dstY_ = buffer + dstOffsetY;
    unsigned અક્ષર* dstV_ = buffer + dstOffsetV;
    unsigned અક્ષર* dstU_ = buffer + dstOffsetU;

    અચલ size_t srcStep = bgr.step;

    માટે (પૂર્ણાંક y = 0; y < bgr.rows; y += 2)
    {
        unsigned અક્ષર* dstY0 = dstY_ + (dstStepY) * y;
        unsigned અક્ષર* dstY1 = dstY0 + dstStepY;

        unsigned અક્ષર* dstV = dstV_ + (dstStepV) * y / 2;
        unsigned અક્ષર* dstU = dstU_ + (dstStepU) * y / 2;

        અચલ unsigned અક્ષર* src0 = bgr.data + (srcStep) * y;
        અચલ unsigned અક્ષર* src1 = src0 + srcStep;

        માટે (પૂર્ણાંક x = 0; x < bgr.cols; x += 2)
        {
            દશાંશ B0 = દશાંશ(src0[(x+0)*NCHANNELS+0]);
            દશાંશ G0 = દશાંશ(src0[(x+0)*NCHANNELS+1]);
            દશાંશ R0 = દશાંશ(src0[(x+0)*NCHANNELS+2]);

            દશાંશ B1 = દશાંશ(src0[(x+1)*NCHANNELS+0]);
            દશાંશ G1 = દશાંશ(src0[(x+1)*NCHANNELS+1]);
            દશાંશ R1 = દશાંશ(src0[(x+1)*NCHANNELS+2]);

            દશાંશ B2 = દશાંશ(src1[(x+0)*NCHANNELS+0]);
            દશાંશ G2 = દશાંશ(src1[(x+0)*NCHANNELS+1]);
            દશાંશ R2 = દશાંશ(src1[(x+0)*NCHANNELS+2]);

            દશાંશ B3 = દશાંશ(src1[(x+1)*NCHANNELS+0]);
            દશાંશ G3 = દશાંશ(src1[(x+1)*NCHANNELS+1]);
            દશાંશ R3 = દશાંશ(src1[(x+1)*NCHANNELS+2]);

            દશાંશ Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            દશાંશ Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            દશાંશ Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            દશાંશ Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            દશાંશ U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            દશાંશ V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<unsigned અક્ષર>(Y0);
            dstY0[x+1] = saturate_cast<unsigned અક્ષર>(Y1);
            dstY1[x+0] = saturate_cast<unsigned અક્ષર>(Y2);
            dstY1[x+1] = saturate_cast<unsigned અક્ષર>(Y3);

            dstU[x/2] = saturate_cast<unsigned અક્ષર>(U);
            dstV[x/2] = saturate_cast<unsigned અક્ષર>(V);
        }
    }
}
#endif // HAVE_VA

શૂન્ય convertToVASurface(VADisplay display, આગત_Array src, VASurfaceID surface, Size size)
{
    CV_UNUSED(display); CV_UNUSED(src); CV_UNUSED(surface); CV_UNUSED(size);
#જો !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#નહીંતર  // !HAVE_VA

    અચલ પૂર્ણાંક stype = CV_8UC3;

    પૂર્ણાંક srcType = src.type();
    CV_Assert(srcType == stype);

    Size srcSize = src.size();
    CV_Assert(srcSize.width == size.width && srcSize.height == size.height);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ખાલી, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    શૂન્ય* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ખાલી, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    તાર્કિક isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ખાલી, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (શૂન્ય*)display << "(surface)");
    જો (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &status);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &status);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ખાલી, ખાલી);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        જો (!ocl::ocl_convert_bgr_to_nv12(clBuffer, (પૂર્ણાંક)u.step[0], u.cols, u.rows, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ખાલી, ખાલી);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    નહીંતર
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        તાર્કિક indirect_buffer = ખોટું;
        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        જો (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            indirect_buffer = સાચું;
            પૂર્ણાંક અંક_formats = va_Max_અંક_Image_Formats(display);
            જો (અંક_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(અંક_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &અંક_formats);
            જો (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            માટે (auto &fmt : fmt_list){
                જો (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    તોડો;
                }
            }
            જો (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            જો (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        unsigned અક્ષર* buffer = 0;
        status = vaMapBuffer(display, image.buf, (શૂન્ય **)&buffer);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        જો (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_to_nv12(image, m, buffer);
        જો (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_to_yv12(image, m, buffer);
        નહીંતર
            CV_Check((પૂર્ણાંક)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        જો (indirect_buffer){
            status = vaPutImage(display, surface, image.image_id, 0, 0, size.width, size.height, 0, 0, size.width, size.height);
            જો (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        status = vaDestroyImage(display, image.image_id);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

શૂન્ય convertFromVASurface(VADisplay display, VASurfaceID surface, Size size, OutputArray dst)
{
    CV_UNUSED(display); CV_UNUSED(surface); CV_UNUSED(dst); CV_UNUSED(size);
#જો !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#નહીંતર  // !HAVE_VA

    અચલ પૂર્ણાંક dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#ifdef HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    જો (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem clBuffer = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int status = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &status);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &status);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_queue q = (cl_command_queue)ocl_context.getQueue().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        status = interop->clEnqueueAcquireVA_APIMediaSurfacesINTEL(q, 2, images, 0, ખાલી, ખાલી);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        જો (!ocl::ocl_convert_nv12_to_bgr(clImageY, clImageUV, clBuffer, (પૂર્ણાંક)u.step[0], u.cols, u.rows))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        status = interop->clEnqueueReleaseVA_APIMediaSurfacesINTEL(q, 2, images, 0, ખાલી, ખાલી);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        status = clFinish(q); // TODO Use events
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        status = clReleaseMemObject(clImageY); // TODO RAII
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        status = clReleaseMemObject(clImageUV);
        જો (status != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    નહીંતર
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.data == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus status = 0;

        status = vaSyncSurface(display, surface);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        status = vaDeriveImage(display, surface, &image);
        જો (status != VA_STATUS_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            પૂર્ણાંક અંક_formats = va_Max_અંક_Image_Formats(display);
            જો (અંક_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(અંક_formats);

            status = vaQueryImageFormats(display, fmt_list.data(), &અંક_formats);
            જો (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = nullptr;
            માટે (auto &fmt : fmt_list){
                જો (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    તોડો;
                }
            }
            જો (selected_format == nullptr)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            status = vaCreateImage(display, selected_format, size.width, size.height, &image);
            જો (status != VA_STATUS_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            status = vaGetImage(display, surface, 0, 0, size.width, size.height, image.image_id);
            જો (status != VA_STATUS_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        unsigned અક્ષર* buffer = 0;
        status = vaMapBuffer(display, image.buf, (શૂન્ય **)&buffer);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        જો (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_to_bgr(image, buffer, m);
        જો (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_to_bgr(image, buffer, m);
        નહીંતર
            CV_Check((પૂર્ણાંક)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        status = vaUnmapBuffer(display, image.buf);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        status = vaDestroyImage(display, image.image_id);
        જો (status != VA_STATUS_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#endif  // !HAVE_VA
}

}} // namespace cv::va_intel
