// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#شامل "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#شامل "opencv2/core/utils/configuration.private.hpp"

فضاینام cv { فضاینام detail {

تعریفنوع VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
تعریفنوع VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
تعریفنوع VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, تهی **pbuf);
تعریفنوع VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
تعریفنوع VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
تعریفنوع صحیح (*FN_va_Max_رقم_Image_Formats)(VADisplay dpy);
تعریفنوع VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, صحیح *رقم_formats);
تعریفنوع VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, صحیح width, صحیح height, VAImage *image);
تعریفنوع VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, صحیح src_x, صحیح src_y, unsigned صحیح src_width, unsigned صحیح src_height, صحیح dest_x, صحیح dest_y, unsigned صحیح dest_width, unsigned صحیح dest_height);
تعریفنوع VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, صحیح x, صحیح y, unsigned صحیح width, unsigned صحیح height, VAImageID image);

ایستا FN_vaDeriveImage fn_vaDeriveImage = پوچ;
ایستا FN_vaDestroyImage fn_vaDestroyImage = پوچ;
ایستا FN_vaMapBuffer fn_vaMapBuffer = پوچ;
ایستا FN_vaSyncSurface fn_vaSyncSurface = پوچ;
ایستا FN_vaUnmapBuffer fn_vaUnmapBuffer = پوچ;
ایستا FN_va_Max_رقم_Image_Formats fn_va_Max_رقم_Image_Formats = پوچ;
ایستا FN_vaQueryImageFormats fn_vaQueryImageFormats = پوچ;
ایستا FN_vaCreateImage fn_vaCreateImage = پوچ;
ایستا FN_vaPutImage fn_vaPutImage = پوچ;
ایستا FN_vaGetImage fn_vaGetImage = پوچ;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_رقم_Image_Formats fn_va_Max_رقم_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


ایستا std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ثابت std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    اگر (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        بازگشت lib;
    }
    ایستا ثابت نویسه* ثابت candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    برای (صحیح i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        اگر (lib->isLoaded())
            بشکن;
    }
    بازگشت lib;
}
ایستا تهی init_libva()
{
    ایستا منطقی initialized = نادرست;
    ایستا auto library = loadLibVA();
    اگر (!initialized)
    {
        اگر (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_نماد(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_نماد(#name)); \
        اگر (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = درست; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_نماد(vaDeriveImage);
        VA_LOAD_نماد(vaDestroyImage);
        VA_LOAD_نماد(vaMapBuffer);
        VA_LOAD_نماد(vaSyncSurface);
        VA_LOAD_نماد(vaUnmapBuffer);
        VA_LOAD_نماد(va_Max_رقم_Image_Formats);
        VA_LOAD_نماد(vaQueryImageFormats);
        VA_LOAD_نماد(vaCreateImage);
        VA_LOAD_نماد(vaPutImage);
        VA_LOAD_نماد(vaGetImage);
        initialized = درست;
    }
    اگر (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
