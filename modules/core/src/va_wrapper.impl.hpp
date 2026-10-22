// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#অন্তর্ভাব "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#অন্তর্ভাব "opencv2/core/utils/configuration.private.hpp"

নামাকাশ cv { নামাকাশ detail {

প্রকারনির্দেশ VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
প্রকারনির্দেশ VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
প্রকারনির্দেশ VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, শূন্য **pbuf);
প্রকারনির্দেশ VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
প্রকারনির্দেশ VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
প্রকারনির্দেশ পূর্ণাঙ্ক (*FN_va_Max_অঙ্ক_Image_Formats)(VADisplay dpy);
প্রকারনির্দেশ VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, পূর্ণাঙ্ক *অঙ্ক_formats);
প্রকারনির্দেশ VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, পূর্ণাঙ্ক width, পূর্ণাঙ্ক height, VAImage *image);
প্রকারনির্দেশ VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, পূর্ণাঙ্ক src_x, পূর্ণাঙ্ক src_y, unsigned পূর্ণাঙ্ক src_width, unsigned পূর্ণাঙ্ক src_height, পূর্ণাঙ্ক dest_x, পূর্ণাঙ্ক dest_y, unsigned পূর্ণাঙ্ক dest_width, unsigned পূর্ণাঙ্ক dest_height);
প্রকারনির্দেশ VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, পূর্ণাঙ্ক x, পূর্ণাঙ্ক y, unsigned পূর্ণাঙ্ক width, unsigned পূর্ণাঙ্ক height, VAImageID image);

স্থির FN_vaDeriveImage fn_vaDeriveImage = রিক্ত;
স্থির FN_vaDestroyImage fn_vaDestroyImage = রিক্ত;
স্থির FN_vaMapBuffer fn_vaMapBuffer = রিক্ত;
স্থির FN_vaSyncSurface fn_vaSyncSurface = রিক্ত;
স্থির FN_vaUnmapBuffer fn_vaUnmapBuffer = রিক্ত;
স্থির FN_va_Max_অঙ্ক_Image_Formats fn_va_Max_অঙ্ক_Image_Formats = রিক্ত;
স্থির FN_vaQueryImageFormats fn_vaQueryImageFormats = রিক্ত;
স্থির FN_vaCreateImage fn_vaCreateImage = রিক্ত;
স্থির FN_vaPutImage fn_vaPutImage = রিক্ত;
স্থির FN_vaGetImage fn_vaGetImage = রিক্ত;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_অঙ্ক_Image_Formats fn_va_Max_অঙ্ক_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


স্থির std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ধ্রুব std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    যদি (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        প্রত্যাগম lib;
    }
    স্থির ধ্রুব বর্ণ* ধ্রুব candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    প্রতি (পূর্ণাঙ্ক i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        যদি (lib->isLoaded())
            ভঙ্গ;
    }
    প্রত্যাগম lib;
}
স্থির শূন্য init_libva()
{
    স্থির তার্কিক initialized = অসত্য;
    স্থির auto library = loadLibVA();
    যদি (!initialized)
    {
        যদি (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_প্রতীক(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_প্রতীক(#name)); \
        যদি (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = সত্য; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_প্রতীক(vaDeriveImage);
        VA_LOAD_প্রতীক(vaDestroyImage);
        VA_LOAD_প্রতীক(vaMapBuffer);
        VA_LOAD_প্রতীক(vaSyncSurface);
        VA_LOAD_প্রতীক(vaUnmapBuffer);
        VA_LOAD_প্রতীক(va_Max_অঙ্ক_Image_Formats);
        VA_LOAD_প্রতীক(vaQueryImageFormats);
        VA_LOAD_প্রতীক(vaCreateImage);
        VA_LOAD_প্রতীক(vaPutImage);
        VA_LOAD_প্রতীক(vaGetImage);
        initialized = সত্য;
    }
    যদি (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
