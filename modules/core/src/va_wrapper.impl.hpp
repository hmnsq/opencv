// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#अन्तर्भाव "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#अन्तर्भाव "opencv2/core/utils/configuration.private.hpp"

नामाकाश cv { नामाकाश detail {

प्रकारनिर्देश VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
प्रकारनिर्देश VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
प्रकारनिर्देश VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, शून्य **pbuf);
प्रकारनिर्देश VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
प्रकारनिर्देश VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
प्रकारनिर्देश पूर्णाङ्क (*FN_va_Max_अंक_Image_Formats)(VADisplay dpy);
प्रकारनिर्देश VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, पूर्णाङ्क *अंक_formats);
प्रकारनिर्देश VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, पूर्णाङ्क width, पूर्णाङ्क height, VAImage *image);
प्रकारनिर्देश VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, पूर्णाङ्क src_x, पूर्णाङ्क src_y, unsigned पूर्णाङ्क src_width, unsigned पूर्णाङ्क src_height, पूर्णाङ्क dest_x, पूर्णाङ्क dest_y, unsigned पूर्णाङ्क dest_width, unsigned पूर्णाङ्क dest_height);
प्रकारनिर्देश VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, पूर्णाङ्क x, पूर्णाङ्क y, unsigned पूर्णाङ्क width, unsigned पूर्णाङ्क height, VAImageID image);

स्थिर FN_vaDeriveImage fn_vaDeriveImage = रिक्त;
स्थिर FN_vaDestroyImage fn_vaDestroyImage = रिक्त;
स्थिर FN_vaMapBuffer fn_vaMapBuffer = रिक्त;
स्थिर FN_vaSyncSurface fn_vaSyncSurface = रिक्त;
स्थिर FN_vaUnmapBuffer fn_vaUnmapBuffer = रिक्त;
स्थिर FN_va_Max_अंक_Image_Formats fn_va_Max_अंक_Image_Formats = रिक्त;
स्थिर FN_vaQueryImageFormats fn_vaQueryImageFormats = रिक्त;
स्थिर FN_vaCreateImage fn_vaCreateImage = रिक्त;
स्थिर FN_vaPutImage fn_vaPutImage = रिक्त;
स्थिर FN_vaGetImage fn_vaGetImage = रिक्त;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_अंक_Image_Formats fn_va_Max_अंक_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


स्थिर std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ध्रुव std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    यदि (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        प्रत्यागम lib;
    }
    स्थिर ध्रुव वर्ण* ध्रुव candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    प्रति (पूर्णाङ्क i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        यदि (lib->isLoaded())
            भङ्ग;
    }
    प्रत्यागम lib;
}
स्थिर शून्य init_libva()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर auto library = loadLibVA();
    यदि (!initialized)
    {
        यदि (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_चिन्ह(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_चिन्ह(#name)); \
        यदि (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = सत्य; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_चिन्ह(vaDeriveImage);
        VA_LOAD_चिन्ह(vaDestroyImage);
        VA_LOAD_चिन्ह(vaMapBuffer);
        VA_LOAD_चिन्ह(vaSyncSurface);
        VA_LOAD_चिन्ह(vaUnmapBuffer);
        VA_LOAD_चिन्ह(va_Max_अंक_Image_Formats);
        VA_LOAD_चिन्ह(vaQueryImageFormats);
        VA_LOAD_चिन्ह(vaCreateImage);
        VA_LOAD_चिन्ह(vaPutImage);
        VA_LOAD_चिन्ह(vaGetImage);
        initialized = सत्य;
    }
    यदि (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
