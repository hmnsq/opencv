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
تعريفنوع VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_نشانہ);
تعريفنوع VAStatus (*FN_va_Unmap_بفر)(VADisplay dpy, VABufferID buf_id);
تعريفنوع صحيح (*FN_va_Max_عدد_Image_Formats)(VADisplay dpy);
تعريفنوع VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, صحيح *عدد_formats);
تعريفنوع VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, صحيح چوڑائی, صحيح اچائی, VAImage *image);
تعريفنوع VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, صحيح src_x, صحيح src_y, بنانشانی صحيح src_چوڑائی, بنانشانی صحيح src_اچائی, صحيح dest_x, صحيح dest_y, بنانشانی صحيح dest_چوڑائی, بنانشانی صحيح dest_اچائی);
تعريفنوع VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, صحيح x, صحيح y, بنانشانی صحيح چوڑائی, بنانشانی صحيح اچائی, VAImageID image);

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

#تعریف_کرو vaDeriveImage fn_vaDeriveImage
#تعریف_کرو vaDestroyImage fn_vaDestroyImage
#تعریف_کرو va_Map_بفر fn_va_Map_بفر
#تعریف_کرو vaSyncSurface fn_vaSyncSurface
#تعریف_کرو va_Unmap_بفر fn_va_Unmap_بفر
#تعریف_کرو va_Max_عدد_Image_Formats fn_va_Max_عدد_Image_Formats
#تعریف_کرو vaQueryImageFormats fn_vaQueryImageFormats
#تعریف_کرو vaCreateImage fn_vaCreateImage
#تعریف_کرو vaPutImage fn_vaPutImage
#تعریف_کرو vaGetImage fn_vaGetImage


ساكن std::shared_ptr<cv::plugin::impl::DynamicLib> لدو_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ثابت std::string env_رستہ = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    إذا (!env_رستہ.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_رستہ);
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
    ساكن خودکار library = لدو_Lib_VA();
    إذا (!initialized)
    {
        إذا (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        خودکار& lib = *library.get();
#تعریف_کرو VA_لدو_علامت(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_علامت(#name)); \
        إذا (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = سچ; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_لدو_علامت(vaDeriveImage);
        VA_لدو_علامت(vaDestroyImage);
        VA_لدو_علامت(va_Map_بفر);
        VA_لدو_علامت(vaSyncSurface);
        VA_لدو_علامت(va_Unmap_بفر);
        VA_لدو_علامت(va_Max_عدد_Image_Formats);
        VA_لدو_علامت(vaQueryImageFormats);
        VA_لدو_علامت(vaCreateImage);
        VA_لدو_علامت(vaPutImage);
        VA_لدو_علامت(vaGetImage);
        initialized = سچ;
    }
    إذا (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
