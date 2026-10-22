// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#അന്തര്ഭാവ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#അന്തര്ഭാവ "opencv2/core/utils/configuration.private.hpp"

നാമാകാശ cv { നാമാകാശ detail {

തരനിർവചനം VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
തരനിർവചനം VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
തരനിർവചനം VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, ശൂന്യം **pbuf);
തരനിർവചനം VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
തരനിർവചനം VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
തരനിർവചനം പൂർണ്ണസംഖ്യ (*FN_va_Max_അക്കം_Image_Formats)(VADisplay dpy);
തരനിർവചനം VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, പൂർണ്ണസംഖ്യ *അക്കം_formats);
തരനിർവചനം VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, പൂർണ്ണസംഖ്യ width, പൂർണ്ണസംഖ്യ height, VAImage *image);
തരനിർവചനം VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, പൂർണ്ണസംഖ്യ src_x, പൂർണ്ണസംഖ്യ src_y, unsigned പൂർണ്ണസംഖ്യ src_width, unsigned പൂർണ്ണസംഖ്യ src_height, പൂർണ്ണസംഖ്യ dest_x, പൂർണ്ണസംഖ്യ dest_y, unsigned പൂർണ്ണസംഖ്യ dest_width, unsigned പൂർണ്ണസംഖ്യ dest_height);
തരനിർവചനം VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, പൂർണ്ണസംഖ്യ x, പൂർണ്ണസംഖ്യ y, unsigned പൂർണ്ണസംഖ്യ width, unsigned പൂർണ്ണസംഖ്യ height, VAImageID image);

സ്ഥിതം FN_vaDeriveImage fn_vaDeriveImage = ശൂന്യ;
സ്ഥിതം FN_vaDestroyImage fn_vaDestroyImage = ശൂന്യ;
സ്ഥിതം FN_vaMapBuffer fn_vaMapBuffer = ശൂന്യ;
സ്ഥിതം FN_vaSyncSurface fn_vaSyncSurface = ശൂന്യ;
സ്ഥിതം FN_vaUnmapBuffer fn_vaUnmapBuffer = ശൂന്യ;
സ്ഥിതം FN_va_Max_അക്കം_Image_Formats fn_va_Max_അക്കം_Image_Formats = ശൂന്യ;
സ്ഥിതം FN_vaQueryImageFormats fn_vaQueryImageFormats = ശൂന്യ;
സ്ഥിതം FN_vaCreateImage fn_vaCreateImage = ശൂന്യ;
സ്ഥിതം FN_vaPutImage fn_vaPutImage = ശൂന്യ;
സ്ഥിതം FN_vaGetImage fn_vaGetImage = ശൂന്യ;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_അക്കം_Image_Formats fn_va_Max_അക്കം_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


സ്ഥിതം std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    സ്ഥിരം std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    എങ്കിൽ (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        തിരികെ lib;
    }
    സ്ഥിതം സ്ഥിരം അക്ഷരം* സ്ഥിരം candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    വേണ്ടി (പൂർണ്ണസംഖ്യ i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        എങ്കിൽ (lib->isLoaded())
            നിർത്തുക;
    }
    തിരികെ lib;
}
സ്ഥിതം ശൂന്യം init_libva()
{
    സ്ഥിതം തർക്കം initialized = അസത്യം;
    സ്ഥിതം auto library = loadLibVA();
    എങ്കിൽ (!initialized)
    {
        എങ്കിൽ (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_ചിഹ്നം(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_ചിഹ്നം(#name)); \
        എങ്കിൽ (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = സത്യം; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_ചിഹ്നം(vaDeriveImage);
        VA_LOAD_ചിഹ്നം(vaDestroyImage);
        VA_LOAD_ചിഹ്നം(vaMapBuffer);
        VA_LOAD_ചിഹ്നം(vaSyncSurface);
        VA_LOAD_ചിഹ്നം(vaUnmapBuffer);
        VA_LOAD_ചിഹ്നം(va_Max_അക്കം_Image_Formats);
        VA_LOAD_ചിഹ്നം(vaQueryImageFormats);
        VA_LOAD_ചിഹ്നം(vaCreateImage);
        VA_LOAD_ചിഹ്നം(vaPutImage);
        VA_LOAD_ചിഹ്നം(vaGetImage);
        initialized = സത്യം;
    }
    എങ്കിൽ (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
