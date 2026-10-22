// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/configuration.private.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ detail {

ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, ಶೂನ್ಯ **pbuf);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
ಪ್ರಕಾರನಿರ್ದೇಶ ಪೂರ್ಣಾಙ್ಕ (*FN_va_Max_ಅಂಕೆ_Image_Formats)(VADisplay dpy);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, ಪೂರ್ಣಾಙ್ಕ *ಅಂಕೆ_formats);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, ಪೂರ್ಣಾಙ್ಕ width, ಪೂರ್ಣಾಙ್ಕ height, VAImage *image);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, ಪೂರ್ಣಾಙ್ಕ src_x, ಪೂರ್ಣಾಙ್ಕ src_y, unsigned ಪೂರ್ಣಾಙ್ಕ src_width, unsigned ಪೂರ್ಣಾಙ್ಕ src_height, ಪೂರ್ಣಾಙ್ಕ dest_x, ಪೂರ್ಣಾಙ್ಕ dest_y, unsigned ಪೂರ್ಣಾಙ್ಕ dest_width, unsigned ಪೂರ್ಣಾಙ್ಕ dest_height);
ಪ್ರಕಾರನಿರ್ದೇಶ VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, ಪೂರ್ಣಾಙ್ಕ x, ಪೂರ್ಣಾಙ್ಕ y, unsigned ಪೂರ್ಣಾಙ್ಕ width, unsigned ಪೂರ್ಣಾಙ್ಕ height, VAImageID image);

ಸ್ಥಿರ FN_vaDeriveImage fn_vaDeriveImage = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaDestroyImage fn_vaDestroyImage = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaMapBuffer fn_vaMapBuffer = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaSyncSurface fn_vaSyncSurface = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaUnmapBuffer fn_vaUnmapBuffer = ರಿಕ್ತ;
ಸ್ಥಿರ FN_va_Max_ಅಂಕೆ_Image_Formats fn_va_Max_ಅಂಕೆ_Image_Formats = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaQueryImageFormats fn_vaQueryImageFormats = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaCreateImage fn_vaCreateImage = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaPutImage fn_vaPutImage = ರಿಕ್ತ;
ಸ್ಥಿರ FN_vaGetImage fn_vaGetImage = ರಿಕ್ತ;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_ಅಂಕೆ_Image_Formats fn_va_Max_ಅಂಕೆ_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


ಸ್ಥಿರ std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ಧ್ರುವ std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    ಯದಿ (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        ಪ್ರತ್ಯಾಗಮ lib;
    }
    ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ* ಧ್ರುವ candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    ಪ್ರತಿ (ಪೂರ್ಣಾಙ್ಕ i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        ಯದಿ (lib->isLoaded())
            ಭಙ್ಗ;
    }
    ಪ್ರತ್ಯಾಗಮ lib;
}
ಸ್ಥಿರ ಶೂನ್ಯ init_libva()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಅಸತ್ಯ;
    ಸ್ಥಿರ auto library = loadLibVA();
    ಯದಿ (!initialized)
    {
        ಯದಿ (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_ಚಿಹ್ನೆ(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_ಚಿಹ್ನೆ(#name)); \
        ಯದಿ (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = ಸತ್ಯ; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_ಚಿಹ್ನೆ(vaDeriveImage);
        VA_LOAD_ಚಿಹ್ನೆ(vaDestroyImage);
        VA_LOAD_ಚಿಹ್ನೆ(vaMapBuffer);
        VA_LOAD_ಚಿಹ್ನೆ(vaSyncSurface);
        VA_LOAD_ಚಿಹ್ನೆ(vaUnmapBuffer);
        VA_LOAD_ಚಿಹ್ನೆ(va_Max_ಅಂಕೆ_Image_Formats);
        VA_LOAD_ಚಿಹ್ನೆ(vaQueryImageFormats);
        VA_LOAD_ಚಿಹ್ನೆ(vaCreateImage);
        VA_LOAD_ಚಿಹ್ನೆ(vaPutImage);
        VA_LOAD_ಚಿಹ್ನೆ(vaGetImage);
        initialized = ಸತ್ಯ;
    }
    ಯದಿ (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
