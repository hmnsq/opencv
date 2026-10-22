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
تعريفنوع VAStatus (*FN_va_Map_میانگیر)(VADisplay dpy, VABufferID buf_id, فراغ **pbuf);
تعريفنوع VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_هدف);
تعريفنوع VAStatus (*FN_va_Unmap_میانگیر)(VADisplay dpy, VABufferID buf_id);
تعريفنوع صحيح (*FN_va_Max_رقم_Image_Formats)(VADisplay dpy);
تعريفنوع VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, صحيح *رقم_formats);
تعريفنوع VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, صحيح عرض, صحيح ارتفاع, VAImage *image);
تعريفنوع VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, صحيح src_x, صحيح src_y, بیعلامت صحيح src_عرض, بیعلامت صحيح src_ارتفاع, صحيح dest_x, صحيح dest_y, بیعلامت صحيح dest_عرض, بیعلامت صحيح dest_ارتفاع);
تعريفنوع VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, صحيح x, صحيح y, بیعلامت صحيح عرض, بیعلامت صحيح ارتفاع, VAImageID image);

ساكن FN_vaDeriveImage fn_vaDeriveImage = عدم;
ساكن FN_vaDestroyImage fn_vaDestroyImage = عدم;
ساكن FN_va_Map_میانگیر fn_va_Map_میانگیر = عدم;
ساكن FN_vaSyncSurface fn_vaSyncSurface = عدم;
ساكن FN_va_Unmap_میانگیر fn_va_Unmap_میانگیر = عدم;
ساكن FN_va_Max_رقم_Image_Formats fn_va_Max_رقم_Image_Formats = عدم;
ساكن FN_vaQueryImageFormats fn_vaQueryImageFormats = عدم;
ساكن FN_vaCreateImage fn_vaCreateImage = عدم;
ساكن FN_vaPutImage fn_vaPutImage = عدم;
ساكن FN_vaGetImage fn_vaGetImage = عدم;

#تعریف_کن vaDeriveImage fn_vaDeriveImage
#تعریف_کن vaDestroyImage fn_vaDestroyImage
#تعریف_کن va_Map_میانگیر fn_va_Map_میانگیر
#تعریف_کن vaSyncSurface fn_vaSyncSurface
#تعریف_کن va_Unmap_میانگیر fn_va_Unmap_میانگیر
#تعریف_کن va_Max_رقم_Image_Formats fn_va_Max_رقم_Image_Formats
#تعریف_کن vaQueryImageFormats fn_vaQueryImageFormats
#تعریف_کن vaCreateImage fn_vaCreateImage
#تعریف_کن vaPutImage fn_vaPutImage
#تعریف_کن vaGetImage fn_vaGetImage


ساكن std::shared_ptr<cv::plugin::impl::DynamicLib> بارگیری_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ثابت std::string env_مسیر = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    إذا (!env_مسیر.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_مسیر);
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
    ساكن خودکار library = بارگیری_Lib_VA();
    إذا (!initialized)
    {
        إذا (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        خودکار& lib = *library.get();
#تعریف_کن VA_بارگیری_نماد(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_نماد(#name)); \
        إذا (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = درست; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_بارگیری_نماد(vaDeriveImage);
        VA_بارگیری_نماد(vaDestroyImage);
        VA_بارگیری_نماد(va_Map_میانگیر);
        VA_بارگیری_نماد(vaSyncSurface);
        VA_بارگیری_نماد(va_Unmap_میانگیر);
        VA_بارگیری_نماد(va_Max_رقم_Image_Formats);
        VA_بارگیری_نماد(vaQueryImageFormats);
        VA_بارگیری_نماد(vaCreateImage);
        VA_بارگیری_نماد(vaPutImage);
        VA_بارگیری_نماد(vaGetImage);
        initialized = درست;
    }
    إذا (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
