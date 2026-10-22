// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#شامل "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#شامل "opencv2/core/utils/configuration.private.hpp"

نامخلا cv { نامخلا detail {

قسمتعریف VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
قسمتعریف VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
قسمتعریف VAStatus (*FN_va_Map_بفر)(VADisplay dpy, VABufferID buf_id, خالی **pbuf);
قسمتعریف VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_ہدف);
قسمتعریف VAStatus (*FN_va_Unmap_بفر)(VADisplay dpy, VABufferID buf_id);
قسمتعریف صحیح (*FN_va_Max_عدد_Image_Formats)(VADisplay dpy);
قسمتعریف VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, صحیح *عدد_formats);
قسمتعریف VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, صحیح چوڑائی, صحیح اونچائی, VAImage *image);
قسمتعریف VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, صحیح src_x, صحیح src_y, غیرعلامتی صحیح src_چوڑائی, غیرعلامتی صحیح src_اونچائی, صحیح dest_x, صحیح dest_y, غیرعلامتی صحیح dest_چوڑائی, غیرعلامتی صحیح dest_اونچائی);
قسمتعریف VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, صحیح x, صحیح y, غیرعلامتی صحیح چوڑائی, غیرعلامتی صحیح اونچائی, VAImageID image);

ساکن FN_vaDeriveImage fn_vaDeriveImage = تہی;
ساکن FN_vaDestroyImage fn_vaDestroyImage = تہی;
ساکن FN_va_Map_بفر fn_va_Map_بفر = تہی;
ساکن FN_vaSyncSurface fn_vaSyncSurface = تہی;
ساکن FN_va_Unmap_بفر fn_va_Unmap_بفر = تہی;
ساکن FN_va_Max_عدد_Image_Formats fn_va_Max_عدد_Image_Formats = تہی;
ساکن FN_vaQueryImageFormats fn_vaQueryImageFormats = تہی;
ساکن FN_vaCreateImage fn_vaCreateImage = تہی;
ساکن FN_vaPutImage fn_vaPutImage = تہی;
ساکن FN_vaGetImage fn_vaGetImage = تہی;

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


ساکن std::shared_ptr<cv::plugin::impl::DynamicLib> بھرو_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ثابت std::string env_راستہ = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    اگر (!env_راستہ.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_راستہ);
        واپس lib;
    }
    ساکن ثابت حرف* ثابت candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    برائے (صحیح i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        اگر (lib->isLoaded())
            توڑو;
    }
    واپس lib;
}
ساکن خالی init_libva()
{
    ساکن منطقی initialized = جھوٹ;
    ساکن خودکار library = بھرو_Lib_VA();
    اگر (!initialized)
    {
        اگر (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        خودکار& lib = *library.get();
#تعریف_کرو VA_بھرو_علامت(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_علامت(#name)); \
        اگر (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = سچ; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_بھرو_علامت(vaDeriveImage);
        VA_بھرو_علامت(vaDestroyImage);
        VA_بھرو_علامت(va_Map_بفر);
        VA_بھرو_علامت(vaSyncSurface);
        VA_بھرو_علامت(va_Unmap_بفر);
        VA_بھرو_علامت(va_Max_عدد_Image_Formats);
        VA_بھرو_علامت(vaQueryImageFormats);
        VA_بھرو_علامت(vaCreateImage);
        VA_بھرو_علامت(vaPutImage);
        VA_بھرو_علامت(vaGetImage);
        initialized = سچ;
    }
    اگر (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
