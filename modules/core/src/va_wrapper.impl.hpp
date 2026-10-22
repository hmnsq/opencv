// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#அந்தர்பாவ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#அந்தர்பாவ "opencv2/core/utils/configuration.private.hpp"

நாமாகாஶ cv { நாமாகாஶ detail {

வகைவரை VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
வகைவரை VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
வகைவரை VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, வெறுமை **pbuf);
வகைவரை VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
வகைவரை VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
வகைவரை முழு (*FN_va_Max_இலக்கம்_Image_Formats)(VADisplay dpy);
வகைவரை VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, முழு *இலக்கம்_formats);
வகைவரை VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, முழு width, முழு height, VAImage *image);
வகைவரை VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, முழு src_x, முழு src_y, unsigned முழு src_width, unsigned முழு src_height, முழு dest_x, முழு dest_y, unsigned முழு dest_width, unsigned முழு dest_height);
வகைவரை VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, முழு x, முழு y, unsigned முழு width, unsigned முழு height, VAImageID image);

நிலையான FN_vaDeriveImage fn_vaDeriveImage = வெற்று;
நிலையான FN_vaDestroyImage fn_vaDestroyImage = வெற்று;
நிலையான FN_vaMapBuffer fn_vaMapBuffer = வெற்று;
நிலையான FN_vaSyncSurface fn_vaSyncSurface = வெற்று;
நிலையான FN_vaUnmapBuffer fn_vaUnmapBuffer = வெற்று;
நிலையான FN_va_Max_இலக்கம்_Image_Formats fn_va_Max_இலக்கம்_Image_Formats = வெற்று;
நிலையான FN_vaQueryImageFormats fn_vaQueryImageFormats = வெற்று;
நிலையான FN_vaCreateImage fn_vaCreateImage = வெற்று;
நிலையான FN_vaPutImage fn_vaPutImage = வெற்று;
நிலையான FN_vaGetImage fn_vaGetImage = வெற்று;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_இலக்கம்_Image_Formats fn_va_Max_இலக்கம்_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


நிலையான std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    மாறிலி std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    எனில் (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        திரும்பு lib;
    }
    நிலையான மாறிலி எழுத்து* மாறிலி candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    க்கு (முழு i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        எனில் (lib->isLoaded())
            நிறுத்து;
    }
    திரும்பு lib;
}
நிலையான வெறுமை init_libva()
{
    நிலையான தருக்க initialized = பொய்;
    நிலையான auto library = loadLibVA();
    எனில் (!initialized)
    {
        எனில் (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_குறியீடு(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_குறியீடு(#name)); \
        எனில் (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = உண்மை; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_குறியீடு(vaDeriveImage);
        VA_LOAD_குறியீடு(vaDestroyImage);
        VA_LOAD_குறியீடு(vaMapBuffer);
        VA_LOAD_குறியீடு(vaSyncSurface);
        VA_LOAD_குறியீடு(vaUnmapBuffer);
        VA_LOAD_குறியீடு(va_Max_இலக்கம்_Image_Formats);
        VA_LOAD_குறியீடு(vaQueryImageFormats);
        VA_LOAD_குறியீடு(vaCreateImage);
        VA_LOAD_குறியீடு(vaPutImage);
        VA_LOAD_குறியீடு(vaGetImage);
        initialized = உண்மை;
    }
    எனில் (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
