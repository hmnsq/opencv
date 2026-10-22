// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#ضمن "opencv2/core/utils/configuration.private.hpp"

نطاق cv { نطاق detail {

تعريفنوع VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
تعريفنوع VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
تعريفنوع VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, فراغ **pbuf);
تعريفنوع VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
تعريفنوع VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
تعريفنوع صحيح (*FN_va_Max_لمب_Image_Formats)(VADisplay dpy);
تعريفنوع VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, صحيح *لمب_formats);
تعريفنوع VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, صحيح width, صحيح height, VAImage *image);
تعريفنوع VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, صحيح src_x, صحيح src_y, unsigned صحيح src_width, unsigned صحيح src_height, صحيح dest_x, صحيح dest_y, unsigned صحيح dest_width, unsigned صحيح dest_height);
تعريفنوع VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, صحيح x, صحيح y, unsigned صحيح width, unsigned صحيح height, VAImageID image);

ساكن FN_vaDeriveImage fn_vaDeriveImage = عدم;
ساكن FN_vaDestroyImage fn_vaDestroyImage = عدم;
ساكن FN_vaMapBuffer fn_vaMapBuffer = عدم;
ساكن FN_vaSyncSurface fn_vaSyncSurface = عدم;
ساكن FN_vaUnmapBuffer fn_vaUnmapBuffer = عدم;
ساكن FN_va_Max_لمب_Image_Formats fn_va_Max_لمب_Image_Formats = عدم;
ساكن FN_vaQueryImageFormats fn_vaQueryImageFormats = عدم;
ساكن FN_vaCreateImage fn_vaCreateImage = عدم;
ساكن FN_vaPutImage fn_vaPutImage = عدم;
ساكن FN_vaGetImage fn_vaGetImage = عدم;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_لمب_Image_Formats fn_va_Max_لمب_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


ساكن std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ثابت std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    إذا (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        أرجع lib;
    }
    ساكن ثابت حرف* ثابت candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    لكل (صحيح i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        إذا (lib->isLoaded())
            اكسر;
    }
    أرجع lib;
}
ساكن فراغ init_libva()
{
    ساكن منطقي initialized = خطأ;
    ساكن auto library = loadLibVA();
    إذا (!initialized)
    {
        إذا (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_علام(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_علام(#name)); \
        إذا (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = true; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_علام(vaDeriveImage);
        VA_LOAD_علام(vaDestroyImage);
        VA_LOAD_علام(vaMapBuffer);
        VA_LOAD_علام(vaSyncSurface);
        VA_LOAD_علام(vaUnmapBuffer);
        VA_LOAD_علام(va_Max_لمب_Image_Formats);
        VA_LOAD_علام(vaQueryImageFormats);
        VA_LOAD_علام(vaCreateImage);
        VA_LOAD_علام(vaPutImage);
        VA_LOAD_علام(vaGetImage);
        initialized = true;
    }
    إذا (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
