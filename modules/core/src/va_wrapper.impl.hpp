// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/configuration.private.hpp"

ନାମାକାଶ cv { ନାମାକାଶ detail {

ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, ଶୂନ୍ୟ **pbuf);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
ପ୍ରକାରବ୍ୟାଖ୍ୟା ପୂର୍ଣ୍ଣସଂଖ୍ୟା (*FN_va_Max_ଅଙ୍କ_Image_Formats)(VADisplay dpy);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *ଅଙ୍କ_formats);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, VAImage *image);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_y, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_width, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା src_height, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_y, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_width, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା dest_height);
ପ୍ରକାରବ୍ୟାଖ୍ୟା VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, ପୂର୍ଣ୍ଣସଂଖ୍ୟା x, ପୂର୍ଣ୍ଣସଂଖ୍ୟା y, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା width, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା height, VAImageID image);

ସ୍ଥିର FN_vaDeriveImage fn_vaDeriveImage = ଖାଲି;
ସ୍ଥିର FN_vaDestroyImage fn_vaDestroyImage = ଖାଲି;
ସ୍ଥିର FN_vaMapBuffer fn_vaMapBuffer = ଖାଲି;
ସ୍ଥିର FN_vaSyncSurface fn_vaSyncSurface = ଖାଲି;
ସ୍ଥିର FN_vaUnmapBuffer fn_vaUnmapBuffer = ଖାଲି;
ସ୍ଥିର FN_va_Max_ଅଙ୍କ_Image_Formats fn_va_Max_ଅଙ୍କ_Image_Formats = ଖାଲି;
ସ୍ଥିର FN_vaQueryImageFormats fn_vaQueryImageFormats = ଖାଲି;
ସ୍ଥିର FN_vaCreateImage fn_vaCreateImage = ଖାଲି;
ସ୍ଥିର FN_vaPutImage fn_vaPutImage = ଖାଲି;
ସ୍ଥିର FN_vaGetImage fn_vaGetImage = ଖାଲି;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_ଅଙ୍କ_Image_Formats fn_va_Max_ଅଙ୍କ_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


ସ୍ଥିର std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ଅଚଳ std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    ଯଦି (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        ଫେରନ୍ତୁ lib;
    }
    ସ୍ଥିର ଅଚଳ ଅକ୍ଷର* ଅଚଳ candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    ପାଇଁ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        ଯଦି (lib->isLoaded())
            ଭାଙ୍ଗ;
    }
    ଫେରନ୍ତୁ lib;
}
ସ୍ଥିର ଶୂନ୍ୟ init_libva()
{
    ସ୍ଥିର ତାର୍କିକ initialized = ମିଥ୍ୟା;
    ସ୍ଥିର auto library = loadLibVA();
    ଯଦି (!initialized)
    {
        ଯଦି (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_ପ୍ରତୀକ(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_ପ୍ରତୀକ(#name)); \
        ଯଦି (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = ସତ୍ୟ; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_ପ୍ରତୀକ(vaDeriveImage);
        VA_LOAD_ପ୍ରତୀକ(vaDestroyImage);
        VA_LOAD_ପ୍ରତୀକ(vaMapBuffer);
        VA_LOAD_ପ୍ରତୀକ(vaSyncSurface);
        VA_LOAD_ପ୍ରତୀକ(vaUnmapBuffer);
        VA_LOAD_ପ୍ରତୀକ(va_Max_ଅଙ୍କ_Image_Formats);
        VA_LOAD_ପ୍ରତୀକ(vaQueryImageFormats);
        VA_LOAD_ପ୍ରତୀକ(vaCreateImage);
        VA_LOAD_ପ୍ରତୀକ(vaPutImage);
        VA_LOAD_ପ୍ରତୀକ(vaGetImage);
        initialized = ସତ୍ୟ;
    }
    ଯଦି (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
