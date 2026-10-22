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
تعريفنوع VAStatus (*FN_va_Map_بفر)(VADisplay dpy, VABufferID buf_id, فراغ **pbuf);
تعريفنوع VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_موخه);
تعريفنوع VAStatus (*FN_va_Unmap_بفر)(VADisplay dpy, VABufferID buf_id);
تعريفنوع صحيح (*FN_va_Max_عدد_Image_Formats)(VADisplay dpy);
تعريفنوع VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, صحيح *عدد_formats);
تعريفنوع VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, صحيح پلنوالی, صحيح لوړوالی, VAImage *image);
تعريفنوع VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, صحيح src_x, صحيح src_y, بینښه صحيح src_پلنوالی, بینښه صحيح src_لوړوالی, صحيح dest_x, صحيح dest_y, بینښه صحيح dest_پلنوالی, بینښه صحيح dest_لوړوالی);
تعريفنوع VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, صحيح x, صحيح y, بینښه صحيح پلنوالی, بینښه صحيح لوړوالی, VAImageID image);

ساكن FN_vaDeriveImage fn_vaDeriveImage = عدم;
ساكن FN_vaDestroyImage fn_vaDestroyImage = عدم;
ساكن FN_va_Map_بفر fn_va_Map_بفر = عدم;
ساكن FN_vaSyncSurface fn_vaSyncSurface = عدم;
ساكن FN_va_Unmap_بفر fn_va_Unmap_بفر = عدم;
ساكن FN_va_Max_عدد_Image_Formats fn_va_Max_عدد_Image_Formats = عدم;
ساكن FN_vaQueryImageFormats fn_vaQueryImageFormats = عدم;
ساكن FN_vaCreateImage fn_vaCreateImage = عدم;
ساكن FN_vaPutImage fn_vaPutImage = عدم;
ساكن FN_vaGetImage fn_vaGetImage = عدم;

#تعریف_کړه vaDeriveImage fn_vaDeriveImage
#تعریف_کړه vaDestroyImage fn_vaDestroyImage
#تعریف_کړه va_Map_بفر fn_va_Map_بفر
#تعریف_کړه vaSyncSurface fn_vaSyncSurface
#تعریف_کړه va_Unmap_بفر fn_va_Unmap_بفر
#تعریف_کړه va_Max_عدد_Image_Formats fn_va_Max_عدد_Image_Formats
#تعریف_کړه vaQueryImageFormats fn_vaQueryImageFormats
#تعریف_کړه vaCreateImage fn_vaCreateImage
#تعریف_کړه vaPutImage fn_vaPutImage
#تعریف_کړه vaGetImage fn_vaGetImage


ساكن std::shared_ptr<cv::plugin::impl::DynamicLib> بارول_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ثابت std::string env_لار = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    إذا (!env_لار.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_لار);
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
    ساكن خپلکاری library = بارول_Lib_VA();
    إذا (!initialized)
    {
        إذا (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        خپلکاری& lib = *library.get();
#تعریف_کړه VA_بارول_نښه(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_نښه(#name)); \
        إذا (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = ریښتیا; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_بارول_نښه(vaDeriveImage);
        VA_بارول_نښه(vaDestroyImage);
        VA_بارول_نښه(va_Map_بفر);
        VA_بارول_نښه(vaSyncSurface);
        VA_بارول_نښه(va_Unmap_بفر);
        VA_بارول_نښه(va_Max_عدد_Image_Formats);
        VA_بارول_نښه(vaQueryImageFormats);
        VA_بارول_نښه(vaCreateImage);
        VA_بارول_نښه(vaPutImage);
        VA_بارول_نښه(vaGetImage);
        initialized = ریښتیا;
    }
    إذا (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
