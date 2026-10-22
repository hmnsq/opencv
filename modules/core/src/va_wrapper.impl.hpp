// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#ਅਨ੍ਤਰ੍ਭਾਵ "opencv2/core/utils/configuration.private.hpp"

ਨਾਮਾਕਾਸ਼ cv { ਨਾਮਾਕਾਸ਼ detail {

ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_va_Map_ਬਫਰ)(VADisplay dpy, VABufferID buf_id, ਖਾਲੀ **pbuf);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_ਨਿਸ਼ਾਨਾ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_va_Unmap_ਬਫਰ)(VADisplay dpy, VABufferID buf_id);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਪੂਰਨ (*FN_va_Max_ਅੰਕ_Image_Formats)(VADisplay dpy);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, ਪੂਰਨ *ਅੰਕ_formats);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, ਪੂਰਨ ਚੌੜਾਈ, ਪੂਰਨ ਉਚਾਈ, VAImage *image);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, ਪੂਰਨ src_x, ਪੂਰਨ src_y, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ src_ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ src_ਉਚਾਈ, ਪੂਰਨ dest_x, ਪੂਰਨ dest_y, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dest_ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ dest_ਉਚਾਈ);
ਕਿਸਮਪਰਿਭਾਸ਼ਾ VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, ਪੂਰਨ x, ਪੂਰਨ y, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਚੌੜਾਈ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਉਚਾਈ, VAImageID image);

ਸਥਿਰ FN_vaDeriveImage fn_vaDeriveImage = ਨਿਰਾ;
ਸਥਿਰ FN_vaDestroyImage fn_vaDestroyImage = ਨਿਰਾ;
ਸਥਿਰ FN_va_Map_ਬਫਰ fn_va_Map_ਬਫਰ = ਨਿਰਾ;
ਸਥਿਰ FN_vaSyncSurface fn_vaSyncSurface = ਨਿਰਾ;
ਸਥਿਰ FN_va_Unmap_ਬਫਰ fn_va_Unmap_ਬਫਰ = ਨਿਰਾ;
ਸਥਿਰ FN_va_Max_ਅੰਕ_Image_Formats fn_va_Max_ਅੰਕ_Image_Formats = ਨਿਰਾ;
ਸਥਿਰ FN_vaQueryImageFormats fn_vaQueryImageFormats = ਨਿਰਾ;
ਸਥਿਰ FN_vaCreateImage fn_vaCreateImage = ਨਿਰਾ;
ਸਥਿਰ FN_vaPutImage fn_vaPutImage = ਨਿਰਾ;
ਸਥਿਰ FN_vaGetImage fn_vaGetImage = ਨਿਰਾ;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vaDeriveImage fn_vaDeriveImage
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vaDestroyImage fn_vaDestroyImage
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ va_Map_ਬਫਰ fn_va_Map_ਬਫਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vaSyncSurface fn_vaSyncSurface
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ va_Unmap_ਬਫਰ fn_va_Unmap_ਬਫਰ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ va_Max_ਅੰਕ_Image_Formats fn_va_Max_ਅੰਕ_Image_Formats
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vaQueryImageFormats fn_vaQueryImageFormats
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vaCreateImage fn_vaCreateImage
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vaPutImage fn_vaPutImage
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ vaGetImage fn_vaGetImage


ਸਥਿਰ std::shared_ptr<cv::plugin::impl::DynamicLib> ਲੱਦੋ_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ਅਚਲ std::string env_ਰਸਤਾ = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    ਜੇ (!env_ਰਸਤਾ.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_ਰਸਤਾ);
        ਵਾਪਸ lib;
    }
    ਸਥਿਰ ਅਚਲ ਅੱਖਰ* ਅਚਲ candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    ਲਈ (ਪੂਰਨ i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        ਜੇ (lib->isLoaded())
            ਰੋਕੋ;
    }
    ਵਾਪਸ lib;
}
ਸਥਿਰ ਖਾਲੀ init_libva()
{
    ਸਥਿਰ ਤਾਰਕਿਕ initialized = ਝੂਠ;
    ਸਥਿਰ ਸਵੈਚਾਲਿਤ library = ਲੱਦੋ_Lib_VA();
    ਜੇ (!initialized)
    {
        ਜੇ (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        ਸਵੈਚਾਲਿਤ& lib = *library.get();
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_ਚਿੰਨ੍ਹ(#name)); \
        ਜੇ (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = ਸੱਚ; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(vaDeriveImage);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(vaDestroyImage);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(va_Map_ਬਫਰ);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(vaSyncSurface);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(va_Unmap_ਬਫਰ);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(va_Max_ਅੰਕ_Image_Formats);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(vaQueryImageFormats);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(vaCreateImage);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(vaPutImage);
        VA_ਲੱਦੋ_ਚਿੰਨ੍ਹ(vaGetImage);
        initialized = ਸੱਚ;
    }
    ਜੇ (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
