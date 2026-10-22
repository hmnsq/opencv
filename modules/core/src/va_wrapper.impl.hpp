// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/configuration.private.hpp"

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ detail {

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_va_Map_ಬಫರ್)(VADisplay dpy, VABufferID buf_id, ಶೂನ್ಯ **pbuf);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_ಗುರಿ);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_va_Unmap_ಬಫರ್)(VADisplay dpy, VABufferID buf_id);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*FN_va_Max_ಅಂಕೆ_Image_Formats)(VADisplay dpy);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, ಪೂರ್ಣಾಂಕ *ಅಂಕೆ_formats);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, ಪೂರ್ಣಾಂಕ ಅಗಲ, ಪೂರ್ಣಾಂಕ ಎತ್ತರ, VAImage *image);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, ಪೂರ್ಣಾಂಕ src_x, ಪೂರ್ಣಾಂಕ src_y, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ src_ಅಗಲ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ src_ಎತ್ತರ, ಪೂರ್ಣಾಂಕ dest_x, ಪೂರ್ಣಾಂಕ dest_y, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ dest_ಅಗಲ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ dest_ಎತ್ತರ);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, ಪೂರ್ಣಾಂಕ x, ಪೂರ್ಣಾಂಕ y, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಗಲ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಎತ್ತರ, VAImageID image);

ಸ್ಥಿರ FN_vaDeriveImage fn_vaDeriveImage = ಖಾಲಿ;
ಸ್ಥಿರ FN_vaDestroyImage fn_vaDestroyImage = ಖಾಲಿ;
ಸ್ಥಿರ FN_va_Map_ಬಫರ್ fn_va_Map_ಬಫರ್ = ಖಾಲಿ;
ಸ್ಥಿರ FN_vaSyncSurface fn_vaSyncSurface = ಖಾಲಿ;
ಸ್ಥಿರ FN_va_Unmap_ಬಫರ್ fn_va_Unmap_ಬಫರ್ = ಖಾಲಿ;
ಸ್ಥಿರ FN_va_Max_ಅಂಕೆ_Image_Formats fn_va_Max_ಅಂಕೆ_Image_Formats = ಖಾಲಿ;
ಸ್ಥಿರ FN_vaQueryImageFormats fn_vaQueryImageFormats = ಖಾಲಿ;
ಸ್ಥಿರ FN_vaCreateImage fn_vaCreateImage = ಖಾಲಿ;
ಸ್ಥಿರ FN_vaPutImage fn_vaPutImage = ಖಾಲಿ;
ಸ್ಥಿರ FN_vaGetImage fn_vaGetImage = ಖಾಲಿ;

#ವ್ಯಾಖ್ಯಾನಿಸು vaDeriveImage fn_vaDeriveImage
#ವ್ಯಾಖ್ಯಾನಿಸು vaDestroyImage fn_vaDestroyImage
#ವ್ಯಾಖ್ಯಾನಿಸು va_Map_ಬಫರ್ fn_va_Map_ಬಫರ್
#ವ್ಯಾಖ್ಯಾನಿಸು vaSyncSurface fn_vaSyncSurface
#ವ್ಯಾಖ್ಯಾನಿಸು va_Unmap_ಬಫರ್ fn_va_Unmap_ಬಫರ್
#ವ್ಯಾಖ್ಯಾನಿಸು va_Max_ಅಂಕೆ_Image_Formats fn_va_Max_ಅಂಕೆ_Image_Formats
#ವ್ಯಾಖ್ಯಾನಿಸು vaQueryImageFormats fn_vaQueryImageFormats
#ವ್ಯಾಖ್ಯಾನಿಸು vaCreateImage fn_vaCreateImage
#ವ್ಯಾಖ್ಯಾನಿಸು vaPutImage fn_vaPutImage
#ವ್ಯಾಖ್ಯಾನಿಸು vaGetImage fn_vaGetImage


ಸ್ಥಿರ std::shared_ptr<cv::plugin::impl::DynamicLib> ತುಂಬು_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ಸ್ಥಿರಾಂಕ std::string env_ಮಾರ್ಗ = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    ಆದರೆ (!env_ಮಾರ್ಗ.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_ಮಾರ್ಗ);
        ಹಿಂತಿರುಗಿ lib;
    }
    ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ* ಸ್ಥಿರಾಂಕ candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    ಗಾಗಿ (ಪೂರ್ಣಾಂಕ i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        ಆದರೆ (lib->isLoaded())
            ನಿಲ್ಲಿಸು;
    }
    ಹಿಂತಿರುಗಿ lib;
}
ಸ್ಥಿರ ಶೂನ್ಯ init_libva()
{
    ಸ್ಥಿರ ತಾರ್ಕಿಕ initialized = ಸುಳ್ಳು;
    ಸ್ಥಿರ ಸ್ವಯಂಚಾಲಿತ library = ತುಂಬು_Lib_VA();
    ಆದರೆ (!initialized)
    {
        ಆದರೆ (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        ಸ್ವಯಂಚಾಲಿತ& lib = *library.get();
#ವ್ಯಾಖ್ಯಾನಿಸು VA_ತುಂಬು_ಚಿಹ್ನೆ(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_ಚಿಹ್ನೆ(#name)); \
        ಆದರೆ (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = ನಿಜ; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_ತುಂಬು_ಚಿಹ್ನೆ(vaDeriveImage);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(vaDestroyImage);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(va_Map_ಬಫರ್);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(vaSyncSurface);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(va_Unmap_ಬಫರ್);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(va_Max_ಅಂಕೆ_Image_Formats);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(vaQueryImageFormats);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(vaCreateImage);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(vaPutImage);
        VA_ತುಂಬು_ಚಿಹ್ನೆ(vaGetImage);
        initialized = ನಿಜ;
    }
    ಆದರೆ (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
