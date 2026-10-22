// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Copyright (C) 2015, Itseez, Inc., all rights reserved.
// Third party copyrights are property of their respective owners.

#అన్తర్భావ "precomp.hpp"

#అన్తర్భావ <opencv2/core/utils/logger.hpp>

#నిర్వచితమైతే HAVE_VA
#  include <va/va.h>
#లేకపోతే  // HAVE_VA
#  define NO_VA_SUPPORT_ERROR CV_Error(cv::Error::StsBadFunc, "OpenCV was build without VA support (libva)")
#అయితే_ముగింపు // HAVE_VA

ఉపయోగించి నామాకాశ cv;

////////////////////////////////////////////////////////////////////////
// CL-VA Interoperability

#నిర్వచితమైతే HAVE_OPENCL
#  include "opencv2/core/opencl/runtime/opencl_core.hpp"
#  include "opencv2/core.hpp"
#  include "opencv2/core/ocl.hpp"
#  include "opencl_kernels_core.hpp"
#అయితే_ముగింపు // HAVE_OPENCL

#నిర్వచితమైతే HAVE_VA_INTEL
#నిర్వచితమైతే HAVE_VA_INTEL_OLD_శీర్షిక
#  include <CL/va_ext.h>
#లేకపోతే
#  include <CL/cl_va_api_media_sharing_intel.h>
#అయితే_ముగింపు
#అయితే_ముగింపు

#నిర్వచితమైతే HAVE_VA
#నిర్వచితం_కాకపోతే OPENCV_LIBVA_LINK
#అన్తర్భావ "va_wrapper.impl.hpp"
#లేకపోతే
నామాకాశ cv { నామాకాశ detail {
స్థిర శూన్య init_libva() { /* nothing */ }
}}  // namespace
#అయితే_ముగింపు
ఉపయోగించి నామాకాశ cv::detail;
#అయితే_ముగింపు

నామాకాశ cv { నామాకాశ va_intel {

#నిర్వచితమైతే HAVE_VA_INTEL

తరగతి VAAPIInterop : బహిరంగ ocl::Context::UserContext
{
బహిరంగ:
    VAAPIInterop(cl_platform_id platform) {
        clCreateFromVA_APIMediaSurfaceINTEL       = (clCreateFromVA_APIMediaSurfaceINTEL_fn)
                cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clCreateFromVA_APIMediaSurfaceINTEL");
        cl_వరుసలో_చేర్చు_Acquire_VA_APIMedia_Surfaces_INTEL  = (cl_వరుసలో_చేర్చు_Acquire_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueAcquireVA_APIMediaSurfacesINTEL");
        cl_వరుసలో_చేర్చు_Release_VA_APIMedia_Surfaces_INTEL  = (cl_వరుసలో_చేర్చు_Release_VA_APIMedia_Surfaces_INTEL_fn)
                cl_Get_Extension_Function_చిరునామా_For_Platform(platform, "clEnqueueReleaseVA_APIMediaSurfacesINTEL");
        ఒకవేళ (!clCreateFromVA_APIMediaSurfaceINTEL ||
            !cl_వరుసలో_చేర్చు_Acquire_VA_APIMedia_Surfaces_INTEL ||
            !cl_వరుసలో_చేర్చు_Release_VA_APIMedia_Surfaces_INTEL) {
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get extension function for VA-API interop");
        }
    }
    మాయా ~VAAPIInterop() {
    }
    clCreateFromVA_APIMediaSurfaceINTEL_fn       clCreateFromVA_APIMediaSurfaceINTEL;
    cl_వరుసలో_చేర్చు_Acquire_VA_APIMedia_Surfaces_INTEL_fn  cl_వరుసలో_చేర్చు_Acquire_VA_APIMedia_Surfaces_INTEL;
    cl_వరుసలో_చేర్చు_Release_VA_APIMedia_Surfaces_INTEL_fn  cl_వరుసలో_చేర్చు_Release_VA_APIMedia_Surfaces_INTEL;
};

#అయితే_ముగింపు // HAVE_VA_INTEL

నామాకాశ ocl {

Context& initializeContextFromVA(VADisplay display, తార్కిక tryInterop)
{
    CV_వాడని(display); CV_వాడని(tryInterop);
#ఒకవేళ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#లేకపోతే  // !HAVE_VA

#   ifdef HAVE_VA_INTEL
    ఒకవేళ (tryInterop)
    {
        cl_uint అంకె_Platforms;
        cl_int స్థితి_2 = clGetPlatformIDs(0, ఖాళీ, &అంకె_Platforms);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get number of platforms");
        ఒకవేళ (అంకె_Platforms == 0)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: No available platforms");

        std::vector<cl_platform_id> platforms(అంకె_Platforms);
        స్థితి_2 = clGetPlatformIDs(అంకె_Platforms, &platforms[0], ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLInitError, "OpenCL: Can't get platform Id list");

        // For CL-VA interop, we must find platform/device with "cl_intel_va_api_media_sharing" extension.
        // With standard initialization procedure, we should examine platform extension string for that.
        // But in practice, the platform ext string doesn't contain it, while device ext string does.
        // Follow Intel procedure (see tutorial), we should obtain device IDs by extension call.
        // Note that we must obtain function pointers using specific platform ID, and can't provide pointers in advance.
        // So, we iterate and select the first platform, for which we got non-NULL pointers, device, and CL context.

        పూర్ణాంకం దొరికింది = -1;
        cl_context context = 0;
        cl_device_id device = 0;

        కొరకు (పూర్ణాంకం i = 0; i < (పూర్ణాంకం)అంకె_Platforms; ++i)
        {
            // Get extension function pointers
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn clGetDeviceIDsFromVA_APIMediaAdapterINTEL;
            clGetDeviceIDsFromVA_APIMediaAdapterINTEL = (clGetDeviceIDsFromVA_APIMediaAdapterINTEL_fn)
                cl_Get_Extension_Function_చిరునామా_For_Platform(platforms[i], "clGetDeviceIDsFromVA_APIMediaAdapterINTEL");
            ఒకవేళ ((శూన్య*)clGetDeviceIDsFromVA_APIMediaAdapterINTEL == ఖాళీ)
            {
                కొనసాగు;
            }

            // Query device list

            cl_uint అంకె_Devices = 0;

            స్థితి_2 = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, 0, ఖాళీ, &అంకె_Devices);
            ఒకవేళ ((స్థితి_2 != CL_SUCCESS) || !(అంకె_Devices > 0))
                కొనసాగు;
            అంకె_Devices = 1; // OpenCV expects only 1 device
            స్థితి_2 = clGetDeviceIDsFromVA_APIMediaAdapterINTEL(platforms[i], CL_VA_API_DISPLAY_INTEL, display,
                                                               CL_PREFERRED_DEVICES_FOR_VA_API_INTEL, అంకె_Devices, &device, ఖాళీ);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
                కొనసాగు;

            // Creating CL-VA media sharing OpenCL context

            cl_context_properties props[] = {
                CL_CONTEXT_VA_API_DISPLAY_INTEL, (cl_context_properties) display,
                CL_CONTEXT_INTEROP_USER_SYNC, CL_FALSE, // no explicit sync required
                0
            };

            context = clCreateContext(props, అంకె_Devices, &device, ఖాళీ, ఖాళీ, &స్థితి_2);
            ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            {
                clReleaseDevice(device);
            }
            లేకపోతే
            {
                దొరికింది = i;
                ఆపు;
            }
        }

        ఒకవేళ (దొరికింది >= 0)
        {
            cl_platform_id platform = platforms[దొరికింది];
            std::string platformName = PlatformInfo(&platform).name();

            OpenCLExecutionContext clExecCtx;
            ప్రయత్నించు
            {
                clExecCtx = OpenCLExecutionContext::create(platformName, platform, context, device);
                clExecCtx.getContext().సమితి_User_Context(std::make_shared<VAAPIInterop>(platform));
            }
            పట్టుకో (...)
            {
                clReleaseDevice(device);
                clReleaseContext(context);
                విసురు;
            }
            clExecCtx.bind();
            తిరిగి const_cast<Context&>(clExecCtx.getContext());
        }
    }
# endif // HAVE_VA_INTEL
    {
        Context& ctx = Context::getDefault();
        తిరిగి ctx;
    }
#అయితే_ముగింపు  // !HAVE_VA
}

#నిర్వచితమైతే HAVE_VA_INTEL
స్థిర తార్కిక ocl_convert_nv12_గమ్యం_bgr(cl_mem clImageY, cl_mem clImageUV, cl_mem cl_బఫరు, పూర్ణాంకం అడుగు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం అడ్డువరుసలు)
{
    ocl::Kernel k;
    k.create("YUV2BGR_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(clImageY, clImageUV, cl_బఫరు, అడుగు, నిలువుల_సంఖ్య, అడ్డువరుసలు);

    size_t globalsize[] = { (size_t)నిలువుల_సంఖ్య/2, (size_t)అడ్డువరుసలు/2 };
    తిరిగి k.నిరంతరం(2, globalsize, 0, అబద్ధం);
}

స్థిర తార్కిక ocl_convert_bgr_గమ్యం_nv12(cl_mem cl_బఫరు, పూర్ణాంకం అడుగు, పూర్ణాంకం నిలువుల_సంఖ్య, పూర్ణాంకం అడ్డువరుసలు, cl_mem clImageY, cl_mem clImageUV)
{
    ocl::Kernel k;
    k.create("BGR2YUV_NV12_8u", cv::ocl::core::cvtclr_dx_oclsrc, "");
    ఒకవేళ (k.empty())
        తిరిగి అబద్ధం;

    k.వాదనలు(cl_బఫరు, అడుగు, నిలువుల_సంఖ్య, అడ్డువరుసలు, clImageY, clImageUV);

    size_t globalsize[] = { (size_t)నిలువుల_సంఖ్య/2, (size_t)అడ్డువరుసలు/2 };
    తిరిగి k.నిరంతరం(2, globalsize, 0, అబద్ధం);
}
#అయితే_ముగింపు // HAVE_VA_INTEL

} // namespace cv::va_intel::ocl

#ఒకవేళ defined(HAVE_VA)
స్థిరరాశి పూర్ణాంకం NCHANNELS = 3;

స్థిర శూన్య copy_convert_nv12_గమ్యం_bgr(స్థిరరాశి VAImage& image, స్థిరరాశి గుర్తులేని అక్షరం* బఫరు, Mat& bgr)
{
    స్థిరరాశి దశాంశం d1 = 16.0f;
    స్థిరరాశి దశాంశం d2 = 128.0f;

    స్థిర స్థిరరాశి దశాంశం coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    స్థిరరాశి size_t src_స్థానభ్రంశం_Y = image.offsets[0];
    స్థిరరాశి size_t src_స్థానభ్రంశం_UV = image.offsets[1];

    స్థిరరాశి size_t src_అడుగు_Y = image.pitches[0];
    స్థిరరాశి size_t src_అడుగు_UV = image.pitches[1];

    స్థిరరాశి size_t dst_అడుగు_3 = bgr.అడుగు;

    స్థిరరాశి గుర్తులేని అక్షరం* srcY0 = బఫరు + src_స్థానభ్రంశం_Y;
    స్థిరరాశి గుర్తులేని అక్షరం* srcUV = బఫరు + src_స్థానభ్రంశం_UV;

    గుర్తులేని అక్షరం* dst0 = bgr.దత్తాంశం;

    కొరకు (పూర్ణాంకం y = 0; y < bgr.అడ్డువరుసలు; y += 2)
    {
        స్థిరరాశి గుర్తులేని అక్షరం* srcY1 = srcY0 + src_అడుగు_Y;
        గుర్తులేని అక్షరం *dst1 = dst0 + dst_అడుగు_3;

        కొరకు (పూర్ణాంకం x = 0; x < bgr.నిలువుల_సంఖ్య; x += 2)
        {
            దశాంశం Y0 = దశాంశం(srcY0[x+0]);
            దశాంశం Y1 = దశాంశం(srcY0[x+1]);
            దశాంశం Y2 = దశాంశం(srcY1[x+0]);
            దశాంశం Y3 = దశాంశం(srcY1[x+1]);

            దశాంశం U = దశాంశం(srcUV[2*(x/2)+0]) - d2;
            దశాంశం V = దశాంశం(srcUV[2*(x/2)+1]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            దశాంశం ruv = coeffs[4]*V;
            దశాంశం guv = coeffs[3]*V + coeffs[2]*U;
            దశాంశం buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y3 + ruv);
        }

        srcY0 = srcY1 + src_అడుగు_Y;
        srcUV += src_అడుగు_UV;
        dst0 = dst1 + dst_అడుగు_3;
    }
}

స్థిర శూన్య copy_convert_bgr_గమ్యం_nv12(స్థిరరాశి VAImage& image, స్థిరరాశి Mat& bgr, గుర్తులేని అక్షరం* బఫరు)
{
    స్థిరరాశి దశాంశం d1 = 16.0f;
    స్థిరరాశి దశాంశం d2 = 128.0f;

    స్థిర స్థిరరాశి దశాంశం coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    స్థిరరాశి size_t dst_స్థానభ్రంశం_Y = image.offsets[0];
    స్థిరరాశి size_t dst_స్థానభ్రంశం_UV = image.offsets[1];

    స్థిరరాశి size_t dst_అడుగు_Y = image.pitches[0];
    స్థిరరాశి size_t dst_అడుగు_UV = image.pitches[1];

    స్థిరరాశి size_t src_అడుగు_3 = bgr.అడుగు;

    స్థిరరాశి గుర్తులేని అక్షరం* src0 = bgr.దత్తాంశం;

    గుర్తులేని అక్షరం* dstY0 = బఫరు + dst_స్థానభ్రంశం_Y;
    గుర్తులేని అక్షరం* dstUV = బఫరు + dst_స్థానభ్రంశం_UV;

    కొరకు (పూర్ణాంకం y = 0; y < bgr.అడ్డువరుసలు; y += 2)
    {
        స్థిరరాశి గుర్తులేని అక్షరం *src1 = src0 + src_అడుగు_3;
        గుర్తులేని అక్షరం* dstY1 = dstY0 + dst_అడుగు_Y;

        కొరకు (పూర్ణాంకం x = 0; x < bgr.నిలువుల_సంఖ్య; x += 2)
        {
            దశాంశం B0 = దశాంశం(src0[(x+0)*NCHANNELS+0]);
            దశాంశం G0 = దశాంశం(src0[(x+0)*NCHANNELS+1]);
            దశాంశం R0 = దశాంశం(src0[(x+0)*NCHANNELS+2]);

            దశాంశం B1 = దశాంశం(src0[(x+1)*NCHANNELS+0]);
            దశాంశం G1 = దశాంశం(src0[(x+1)*NCHANNELS+1]);
            దశాంశం R1 = దశాంశం(src0[(x+1)*NCHANNELS+2]);

            దశాంశం B2 = దశాంశం(src1[(x+0)*NCHANNELS+0]);
            దశాంశం G2 = దశాంశం(src1[(x+0)*NCHANNELS+1]);
            దశాంశం R2 = దశాంశం(src1[(x+0)*NCHANNELS+2]);

            దశాంశం B3 = దశాంశం(src1[(x+1)*NCHANNELS+0]);
            దశాంశం G3 = దశాంశం(src1[(x+1)*NCHANNELS+1]);
            దశాంశం R3 = దశాంశం(src1[(x+1)*NCHANNELS+2]);

            దశాంశం Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            దశాంశం Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            దశాంశం Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            దశాంశం Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            దశాంశం U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            దశాంశం V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<గుర్తులేని అక్షరం>(Y0);
            dstY0[x+1] = saturate_cast<గుర్తులేని అక్షరం>(Y1);
            dstY1[x+0] = saturate_cast<గుర్తులేని అక్షరం>(Y2);
            dstY1[x+1] = saturate_cast<గుర్తులేని అక్షరం>(Y3);

            dstUV[2*(x/2)+0] = saturate_cast<గుర్తులేని అక్షరం>(U);
            dstUV[2*(x/2)+1] = saturate_cast<గుర్తులేని అక్షరం>(V);
        }

        src0 = src1 + src_అడుగు_3;
        dstY0 = dstY1 + dst_అడుగు_Y;
        dstUV += dst_అడుగు_UV;
    }
}


స్థిర శూన్య copy_convert_yv12_గమ్యం_bgr(స్థిరరాశి VAImage& image, స్థిరరాశి గుర్తులేని అక్షరం* బఫరు, Mat& bgr)
{
    స్థిరరాశి దశాంశం d1 = 16.0f;
    స్థిరరాశి దశాంశం d2 = 128.0f;

    స్థిర స్థిరరాశి దశాంశం coeffs[5] =
        {
            1.163999557f,
            2.017999649f,
            -0.390999794f,
            -0.812999725f,
            1.5959997177f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.అంకె_planes, (size_t)3, "");

    స్థిరరాశి size_t src_స్థానభ్రంశం_Y = image.offsets[0];
    స్థిరరాశి size_t src_స్థానభ్రంశం_V = image.offsets[1];
    స్థిరరాశి size_t src_స్థానభ్రంశం_U = image.offsets[2];

    స్థిరరాశి size_t src_అడుగు_Y = image.pitches[0];
    స్థిరరాశి size_t src_అడుగు_U = image.pitches[1];
    స్థిరరాశి size_t src_అడుగు_V = image.pitches[2];

    స్థిరరాశి size_t dst_అడుగు_3 = bgr.అడుగు;

    స్థిరరాశి గుర్తులేని అక్షరం* srcY_ = బఫరు + src_స్థానభ్రంశం_Y;
    స్థిరరాశి గుర్తులేని అక్షరం* srcV_ = బఫరు + src_స్థానభ్రంశం_V;
    స్థిరరాశి గుర్తులేని అక్షరం* srcU_ = బఫరు + src_స్థానభ్రంశం_U;

    కొరకు (పూర్ణాంకం y = 0; y < bgr.అడ్డువరుసలు; y += 2)
    {
        స్థిరరాశి గుర్తులేని అక్షరం* srcY0 = srcY_ + (src_అడుగు_Y) * y;
        స్థిరరాశి గుర్తులేని అక్షరం* srcY1 = srcY0 + src_అడుగు_Y;

        స్థిరరాశి గుర్తులేని అక్షరం* srcV = srcV_ + (src_అడుగు_V) * y / 2;
        స్థిరరాశి గుర్తులేని అక్షరం* srcU = srcU_ + (src_అడుగు_U) * y / 2;

        గుర్తులేని అక్షరం* dst0 = bgr.దత్తాంశం + (dst_అడుగు_3) * y;
        గుర్తులేని అక్షరం* dst1 = dst0 + dst_అడుగు_3;

        కొరకు (పూర్ణాంకం x = 0; x < bgr.నిలువుల_సంఖ్య; x += 2)
        {
            దశాంశం Y0 = దశాంశం(srcY0[x+0]);
            దశాంశం Y1 = దశాంశం(srcY0[x+1]);
            దశాంశం Y2 = దశాంశం(srcY1[x+0]);
            దశాంశం Y3 = దశాంశం(srcY1[x+1]);

            దశాంశం U = దశాంశం(srcU[x/2]) - d2;
            దశాంశం V = దశాంశం(srcV[x/2]) - d2;

            Y0 = std::max(0.0f, Y0 - d1) * coeffs[0];
            Y1 = std::max(0.0f, Y1 - d1) * coeffs[0];
            Y2 = std::max(0.0f, Y2 - d1) * coeffs[0];
            Y3 = std::max(0.0f, Y3 - d1) * coeffs[0];

            దశాంశం ruv = coeffs[4]*V;
            దశాంశం guv = coeffs[3]*V + coeffs[2]*U;
            దశాంశం buv = coeffs[1]*U;

            dst0[(x+0)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y0 + buv);
            dst0[(x+0)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y0 + guv);
            dst0[(x+0)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y0 + ruv);

            dst0[(x+1)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y1 + buv);
            dst0[(x+1)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y1 + guv);
            dst0[(x+1)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y1 + ruv);

            dst1[(x+0)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y2 + buv);
            dst1[(x+0)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y2 + guv);
            dst1[(x+0)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y2 + ruv);

            dst1[(x+1)*NCHANNELS+0] = saturate_cast<గుర్తులేని అక్షరం>(Y3 + buv);
            dst1[(x+1)*NCHANNELS+1] = saturate_cast<గుర్తులేని అక్షరం>(Y3 + guv);
            dst1[(x+1)*NCHANNELS+2] = saturate_cast<గుర్తులేని అక్షరం>(Y3 + ruv);
        }
    }
}

స్థిర శూన్య copy_convert_bgr_గమ్యం_yv12(స్థిరరాశి VAImage& image, స్థిరరాశి Mat& bgr, గుర్తులేని అక్షరం* బఫరు)
{
    స్థిరరాశి దశాంశం d1 = 16.0f;
    స్థిరరాశి దశాంశం d2 = 128.0f;

    స్థిర స్థిరరాశి దశాంశం coeffs[8] =
        {
            0.256999969f,  0.50399971f,   0.09799957f,   -0.1479988098f,
            -0.2909994125f, 0.438999176f, -0.3679990768f, -0.0709991455f
        };

    CV_CheckEQ((size_t)image.format.fourcc, (size_t)VA_FOURCC_YV12, "Unexpected image format");
    CV_CheckEQ((size_t)image.అంకె_planes, (size_t)3, "");

    స్థిరరాశి size_t dst_స్థానభ్రంశం_Y = image.offsets[0];
    స్థిరరాశి size_t dst_స్థానభ్రంశం_V = image.offsets[1];
    స్థిరరాశి size_t dst_స్థానభ్రంశం_U = image.offsets[2];

    స్థిరరాశి size_t dst_అడుగు_Y = image.pitches[0];
    స్థిరరాశి size_t dst_అడుగు_U = image.pitches[1];
    స్థిరరాశి size_t dst_అడుగు_V = image.pitches[2];

    గుర్తులేని అక్షరం* dstY_ = బఫరు + dst_స్థానభ్రంశం_Y;
    గుర్తులేని అక్షరం* dstV_ = బఫరు + dst_స్థానభ్రంశం_V;
    గుర్తులేని అక్షరం* dstU_ = బఫరు + dst_స్థానభ్రంశం_U;

    స్థిరరాశి size_t src_అడుగు_3 = bgr.అడుగు;

    కొరకు (పూర్ణాంకం y = 0; y < bgr.అడ్డువరుసలు; y += 2)
    {
        గుర్తులేని అక్షరం* dstY0 = dstY_ + (dst_అడుగు_Y) * y;
        గుర్తులేని అక్షరం* dstY1 = dstY0 + dst_అడుగు_Y;

        గుర్తులేని అక్షరం* dstV = dstV_ + (dst_అడుగు_V) * y / 2;
        గుర్తులేని అక్షరం* dstU = dstU_ + (dst_అడుగు_U) * y / 2;

        స్థిరరాశి గుర్తులేని అక్షరం* src0 = bgr.దత్తాంశం + (src_అడుగు_3) * y;
        స్థిరరాశి గుర్తులేని అక్షరం* src1 = src0 + src_అడుగు_3;

        కొరకు (పూర్ణాంకం x = 0; x < bgr.నిలువుల_సంఖ్య; x += 2)
        {
            దశాంశం B0 = దశాంశం(src0[(x+0)*NCHANNELS+0]);
            దశాంశం G0 = దశాంశం(src0[(x+0)*NCHANNELS+1]);
            దశాంశం R0 = దశాంశం(src0[(x+0)*NCHANNELS+2]);

            దశాంశం B1 = దశాంశం(src0[(x+1)*NCHANNELS+0]);
            దశాంశం G1 = దశాంశం(src0[(x+1)*NCHANNELS+1]);
            దశాంశం R1 = దశాంశం(src0[(x+1)*NCHANNELS+2]);

            దశాంశం B2 = దశాంశం(src1[(x+0)*NCHANNELS+0]);
            దశాంశం G2 = దశాంశం(src1[(x+0)*NCHANNELS+1]);
            దశాంశం R2 = దశాంశం(src1[(x+0)*NCHANNELS+2]);

            దశాంశం B3 = దశాంశం(src1[(x+1)*NCHANNELS+0]);
            దశాంశం G3 = దశాంశం(src1[(x+1)*NCHANNELS+1]);
            దశాంశం R3 = దశాంశం(src1[(x+1)*NCHANNELS+2]);

            దశాంశం Y0 = coeffs[0]*R0 + coeffs[1]*G0 + coeffs[2]*B0 + d1;
            దశాంశం Y1 = coeffs[0]*R1 + coeffs[1]*G1 + coeffs[2]*B1 + d1;
            దశాంశం Y2 = coeffs[0]*R2 + coeffs[1]*G2 + coeffs[2]*B2 + d1;
            దశాంశం Y3 = coeffs[0]*R3 + coeffs[1]*G3 + coeffs[2]*B3 + d1;

            దశాంశం U = coeffs[3]*R0 + coeffs[4]*G0 + coeffs[5]*B0 + d2;
            దశాంశం V = coeffs[5]*R0 + coeffs[6]*G0 + coeffs[7]*B0 + d2;

            dstY0[x+0] = saturate_cast<గుర్తులేని అక్షరం>(Y0);
            dstY0[x+1] = saturate_cast<గుర్తులేని అక్షరం>(Y1);
            dstY1[x+0] = saturate_cast<గుర్తులేని అక్షరం>(Y2);
            dstY1[x+1] = saturate_cast<గుర్తులేని అక్షరం>(Y3);

            dstU[x/2] = saturate_cast<గుర్తులేని అక్షరం>(U);
            dstV[x/2] = saturate_cast<గుర్తులేని అక్షరం>(V);
        }
    }
}
#అయితే_ముగింపు // HAVE_VA

శూన్య convert_గమ్యం_VASurface(VADisplay display, ఆదానం_Array src, VASurfaceID surface, కొలత size)
{
    CV_వాడని(display); CV_వాడని(src); CV_వాడని(surface); CV_వాడని(size);
#ఒకవేళ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#లేకపోతే  // !HAVE_VA

    స్థిరరాశి పూర్ణాంకం stype = CV_8UC3;

    పూర్ణాంకం srcType = src.type();
    CV_Assert(srcType == stype);

    కొలత src_కొలత = src.size();
    CV_Assert(src_కొలత.వెడల్పు == size.వెడల్పు && src_కొలత.ఎత్తు == size.ఎత్తు);

#నిర్వచితమైతే HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    CV_LOG_IF_DEBUG(ఖాళీ, !interop,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VAAPIInterop API. "
        "OpenCL context should be created through initializeContextFromVA()");
    శూన్య* context_display = ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL);
    CV_LOG_IF_INFO(ఖాళీ, interop && !context_display,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - missing VA display, context re-creation is required");
    తార్కిక isValidContextDisplay = (display == context_display);
    CV_LOG_IF_INFO(ఖాళీ, interop && context_display && !isValidContextDisplay,
        "OpenCL/VA_INTEL: Can't interop with current OpenCL context - VA display mismatch: " << context_display << "(context) vs " << (శూన్య*)display << "(surface)");
    ఒకవేళ (isValidContextDisplay && interop)
    {
        UMat u = src.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_READ);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int స్థితి_2 = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 0, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_WRITE_ONLY, &surface, 1, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_వరుస q = (cl_command_వరుస)ocl_context.get_వరుస().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        స్థితి_2 = interop->cl_వరుసలో_చేర్చు_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ఒకవేళ (!ocl::ocl_convert_bgr_గమ్యం_nv12(cl_బఫరు, (పూర్ణాంకం)u.అడుగు[0], u.నిలువుల_సంఖ్య, u.అడ్డువరుసలు, clImageY, clImageUV))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_bgr_to_nv12 failed");
        interop->cl_వరుసలో_చేర్చు_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        స్థితి_2 = cl_ముగింపు(q); // TODO Use events
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        స్థితి_2 = clReleaseMemObject(clImageY); // TODO RAII
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        స్థితి_2 = clReleaseMemObject(clImageUV);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    లేకపోతే
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = src.getMat();

        // TODO Add support for roi
        CV_Assert(m.దత్తాంశం == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus స్థితి_2 = 0;

        స్థితి_2 = vaSyncSurface(display, surface);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        తార్కిక పరోక్ష_బఫరు = అబద్ధం;
        VAImage image;
        స్థితి_2 = vaDeriveImage(display, surface, &image);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS){
            //try vaCreateImage + vaPutImage
            //pick a format
            పరోక్ష_బఫరు = నిజం;
            పూర్ణాంకం అంకె_formats = va_Max_అంకె_Image_Formats(display);
            ఒకవేళ (అంకె_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(అంకె_formats);

            స్థితి_2 = vaQueryImageFormats(display, fmt_list.దత్తాంశం(), &అంకె_formats);
            ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = శూన్యసూచిక;
            కొరకు (స్వయంచాలక &fmt : fmt_list){
                ఒకవేళ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ఆపు;
                }
            }
            ఒకవేళ (selected_format == శూన్యసూచిక)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            స్థితి_2 = vaCreateImage(display, selected_format, size.వెడల్పు, size.ఎత్తు, &image);
            ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

        }

        గుర్తులేని అక్షరం* బఫరు = 0;
        స్థితి_2 = va_Map_బఫరు(display, image.buf, (శూన్య **)&బఫరు);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ఒకవేళ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_bgr_గమ్యం_nv12(image, m, బఫరు);
        ఒకవేళ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_bgr_గమ్యం_yv12(image, m, బఫరు);
        లేకపోతే
            CV_Check((పూర్ణాంకం)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        స్థితి_2 = va_Unmap_బఫరు(display, image.buf);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        ఒకవేళ (పరోక్ష_బఫరు){
            స్థితి_2 = vaPutImage(display, surface, image.image_id, 0, 0, size.వెడల్పు, size.ఎత్తు, 0, 0, size.వెడల్పు, size.ఎత్తు);
            ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        స్థితి_2 = vaDestroyImage(display, image.image_id);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#అయితే_ముగింపు  // !HAVE_VA
}

శూన్య convertFromVASurface(VADisplay display, VASurfaceID surface, కొలత size, OutputArray dst)
{
    CV_వాడని(display); CV_వాడని(surface); CV_వాడని(dst); CV_వాడని(size);
#ఒకవేళ !defined(HAVE_VA)
    NO_VA_SUPPORT_ERROR;
#లేకపోతే  // !HAVE_VA

    స్థిరరాశి పూర్ణాంకం dtype = CV_8UC3;

    // TODO Need to specify ACCESS_WRITE here somehow to prevent useless data copying!
    dst.create(size, dtype);

#నిర్వచితమైతే HAVE_VA_INTEL
    ocl::OpenCLExecutionContext& ocl_context = ocl::OpenCLExecutionContext::getCurrent();
    VAAPIInterop* interop = ocl_context.getContext().getUserContext<VAAPIInterop>().get();
    ఒకవేళ (display == ocl_context.getContext().getOpenCLContextProperty(CL_CONTEXT_VA_API_DISPLAY_INTEL) && interop)
    {
        UMat u = dst.getUMat();

        // TODO Add support for roi
        CV_Assert(u.offset == 0);
        CV_Assert(u.isContinuous());

        cl_mem cl_బఫరు = (cl_mem)u.handle(ACCESS_WRITE);

        cl_context context = (cl_context)ocl_context.getContext().ptr();

        cl_int స్థితి_2 = 0;

        cl_mem clImageY = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 0, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (Y plane)");
        cl_mem clImageUV = interop->clCreateFromVA_APIMediaSurfaceINTEL(context, CL_MEM_READ_ONLY, &surface, 1, &స్థితి_2);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clCreateFromVA_APIMediaSurfaceINTEL failed (UV plane)");

        cl_command_వరుస q = (cl_command_వరుస)ocl_context.get_వరుస().ptr();

        cl_mem images[2] = { clImageY, clImageUV };
        స్థితి_2 = interop->cl_వరుసలో_చేర్చు_Acquire_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueAcquireVA_APIMediaSurfacesINTEL failed");
        ఒకవేళ (!ocl::ocl_convert_nv12_గమ్యం_bgr(clImageY, clImageUV, cl_బఫరు, (పూర్ణాంకం)u.అడుగు[0], u.నిలువుల_సంఖ్య, u.అడ్డువరుసలు))
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: ocl_convert_nv12_to_bgr failed");
        స్థితి_2 = interop->cl_వరుసలో_చేర్చు_Release_VA_APIMedia_Surfaces_INTEL(q, 2, images, 0, ఖాళీ, ఖాళీ);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clEnqueueReleaseVA_APIMediaSurfacesINTEL failed");

        స్థితి_2 = cl_ముగింపు(q); // TODO Use events
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clFinish failed");

        స్థితి_2 = clReleaseMemObject(clImageY); // TODO RAII
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (Y plane)");
        స్థితి_2 = clReleaseMemObject(clImageUV);
        ఒకవేళ (స్థితి_2 != CL_SUCCESS)
            CV_Error(cv::Error::OpenCLApiCallError, "OpenCL: clReleaseMem failed (UV plane)");
    }
    లేకపోతే
# endif // HAVE_VA_INTEL
    {
        init_libva();
        Mat m = dst.getMat();

        // TODO Add support for roi
        CV_Assert(m.దత్తాంశం == m.datastart);
        CV_Assert(m.isContinuous());

        VAStatus స్థితి_2 = 0;

        స్థితి_2 = vaSyncSurface(display, surface);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaSyncSurface failed");

        VAImage image;
        స్థితి_2 = vaDeriveImage(display, surface, &image);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS){
            //try vaCreateImage + vaGetImage
            //pick a format
            పూర్ణాంకం అంకె_formats = va_Max_అంకె_Image_Formats(display);
            ఒకవేళ (అంకె_formats <= 0)
                CV_Error(cv::Error::StsError, "VA-API: vaMaxNumImageFormats failed");
            std::vector<VAImageFormat> fmt_list(అంకె_formats);

            స్థితి_2 = vaQueryImageFormats(display, fmt_list.దత్తాంశం(), &అంకె_formats);
            ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats failed");
            VAImageFormat *selected_format = శూన్యసూచిక;
            కొరకు (స్వయంచాలక &fmt : fmt_list){
                ఒకవేళ (fmt.fourcc == VA_FOURCC_NV12 || fmt.fourcc == VA_FOURCC_YV12){
                    selected_format = &fmt;
                    ఆపు;
                }
            }
            ఒకవేళ (selected_format == శూన్యసూచిక)
                CV_Error(cv::Error::StsError, "VA-API: vaQueryImageFormats did not return a supported format");

            స్థితి_2 = vaCreateImage(display, selected_format, size.వెడల్పు, size.ఎత్తు, &image);
            ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
                CV_Error(cv::Error::StsError, "VA-API: vaCreateImage failed");

            స్థితి_2 = vaGetImage(display, surface, 0, 0, size.వెడల్పు, size.ఎత్తు, image.image_id);
            ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS){
                vaDestroyImage(display, image.image_id);
                CV_Error(cv::Error::StsError, "VA-API: vaPutImage failed");
            }
        }

        గుర్తులేని అక్షరం* బఫరు = 0;
        స్థితి_2 = va_Map_బఫరు(display, image.buf, (శూన్య **)&బఫరు);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaMapBuffer failed");

        ఒకవేళ (image.format.fourcc == VA_FOURCC_NV12)
            copy_convert_nv12_గమ్యం_bgr(image, బఫరు, m);
        ఒకవేళ (image.format.fourcc == VA_FOURCC_YV12)
            copy_convert_yv12_గమ్యం_bgr(image, బఫరు, m);
        లేకపోతే
            CV_Check((పూర్ణాంకం)image.format.fourcc, image.format.fourcc == VA_FOURCC_NV12 || image.format.fourcc == VA_FOURCC_YV12, "Unexpected image format");

        స్థితి_2 = va_Unmap_బఫరు(display, image.buf);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaUnmapBuffer failed");

        స్థితి_2 = vaDestroyImage(display, image.image_id);
        ఒకవేళ (స్థితి_2 != VA_స్థితి_SUCCESS)
            CV_Error(cv::Error::StsError, "VA-API: vaDestroyImage failed");
    }
#అయితే_ముగింపు  // !HAVE_VA
}

}} // namespace cv::va_intel
