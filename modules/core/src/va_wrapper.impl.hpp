// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#अन्तर्भाव "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#अन्तर्भाव "opencv2/core/utils/configuration.private.hpp"

नामाकाश cv { नामाकाश detail {

प्रकारव्याख्या VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
प्रकारव्याख्या VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
प्रकारव्याख्या VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, शून्य **pbuf);
प्रकारव्याख्या VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
प्रकारव्याख्या VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
प्रकारव्याख्या पूर्णांक (*FN_va_Max_अंक_Image_Formats)(VADisplay dpy);
प्रकारव्याख्या VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, पूर्णांक *अंक_formats);
प्रकारव्याख्या VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, पूर्णांक width, पूर्णांक height, VAImage *image);
प्रकारव्याख्या VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, पूर्णांक src_x, पूर्णांक src_y, अचिन्हित पूर्णांक src_width, अचिन्हित पूर्णांक src_height, पूर्णांक dest_x, पूर्णांक dest_y, अचिन्हित पूर्णांक dest_width, अचिन्हित पूर्णांक dest_height);
प्रकारव्याख्या VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, पूर्णांक x, पूर्णांक y, अचिन्हित पूर्णांक width, अचिन्हित पूर्णांक height, VAImageID image);

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
    स्थिरांक std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    जर (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        परत lib;
    }
    स्थिर स्थिरांक अक्षर* स्थिरांक candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    साठी (पूर्णांक i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        जर (lib->isLoaded())
            थांबा;
    }
    परत lib;
}
स्थिर शून्य init_libva()
{
    स्थिर तार्किक initialized = असत्य;
    स्थिर स्वतः library = loadLibVA();
    जर (!initialized)
    {
        जर (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        स्वतः& lib = *library.get();
#define VA_LOAD_चिन्ह(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_चिन्ह(#name)); \
        जर (!fn_ ## name) \
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
    जर (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
