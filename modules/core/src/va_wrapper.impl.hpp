// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#অন্তর্ভাব "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#অন্তর্ভাব "opencv2/core/utils/configuration.private.hpp"

নামাকাশ cv { নামাকাশ detail {

প্রকারলেখ VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
প্রকারলেখ VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
প্রকারলেখ VAStatus (*FN_va_Map_বাফার)(VADisplay dpy, VABufferID buf_id, শূন্য **pbuf);
প্রকারলেখ VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
প্রকারলেখ VAStatus (*FN_va_Unmap_বাফার)(VADisplay dpy, VABufferID buf_id);
প্রকারলেখ পূর্ণসংখ্যা (*FN_va_Max_অঙ্ক_Image_Formats)(VADisplay dpy);
প্রকারলেখ VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, পূর্ণসংখ্যা *অঙ্ক_formats);
প্রকারলেখ VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, পূর্ণসংখ্যা width, পূর্ণসংখ্যা height, VAImage *image);
প্রকারলেখ VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, পূর্ণসংখ্যা src_x, পূর্ণসংখ্যা src_y, unsigned পূর্ণসংখ্যা src_width, unsigned পূর্ণসংখ্যা src_height, পূর্ণসংখ্যা dest_x, পূর্ণসংখ্যা dest_y, unsigned পূর্ণসংখ্যা dest_width, unsigned পূর্ণসংখ্যা dest_height);
প্রকারলেখ VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, পূর্ণসংখ্যা x, পূর্ণসংখ্যা y, unsigned পূর্ণসংখ্যা width, unsigned পূর্ণসংখ্যা height, VAImageID image);

স্থির FN_vaDeriveImage fn_vaDeriveImage = ফাঁকা;
স্থির FN_vaDestroyImage fn_vaDestroyImage = ফাঁকা;
স্থির FN_va_Map_বাফার fn_va_Map_বাফার = ফাঁকা;
স্থির FN_vaSyncSurface fn_vaSyncSurface = ফাঁকা;
স্থির FN_va_Unmap_বাফার fn_va_Unmap_বাফার = ফাঁকা;
স্থির FN_va_Max_অঙ্ক_Image_Formats fn_va_Max_অঙ্ক_Image_Formats = ফাঁকা;
স্থির FN_vaQueryImageFormats fn_vaQueryImageFormats = ফাঁকা;
স্থির FN_vaCreateImage fn_vaCreateImage = ফাঁকা;
স্থির FN_vaPutImage fn_vaPutImage = ফাঁকা;
স্থির FN_vaGetImage fn_vaGetImage = ফাঁকা;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define va_Map_বাফার fn_va_Map_বাফার
#define vaSyncSurface fn_vaSyncSurface
#define va_Unmap_বাফার fn_va_Unmap_বাফার
#define va_Max_অঙ্ক_Image_Formats fn_va_Max_অঙ্ক_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


স্থির std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ধ্রুবক std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    যদি (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        ফেরত lib;
    }
    স্থির ধ্রুবক অক্ষর* ধ্রুবক candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    জন্য (পূর্ণসংখ্যা i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        যদি (lib->isLoaded())
            বিরতি;
    }
    ফেরত lib;
}
স্থির শূন্য init_libva()
{
    স্থির যৌক্তিক initialized = মিথ্যা;
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
        VA_LOAD_প্রতীক(va_Map_বাফার);
        VA_LOAD_প্রতীক(vaSyncSurface);
        VA_LOAD_প্রতীক(va_Unmap_বাফার);
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
