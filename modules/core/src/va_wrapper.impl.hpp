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
प्रकारनिर्देश VAStatus (*FN_va_Map_बफर)(VADisplay dpy, VABufferID buf_id, शून्य **pbuf);
प्रकारनिर्देश VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
प्रकारनिर्देश VAStatus (*FN_va_Unmap_बफर)(VADisplay dpy, VABufferID buf_id);
प्रकारनिर्देश पूर्णाङ्क (*FN_va_Max_अङ्कः_Image_Formats)(VADisplay dpy);
प्रकारनिर्देश VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, पूर्णाङ्क *अङ्कः_formats);
प्रकारनिर्देश VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, पूर्णाङ्क विस्तारः, पूर्णाङ्क height, VAImage *image);
प्रकारनिर्देश VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, पूर्णाङ्क src_x, पूर्णाङ्क src_y, अचिह्नित पूर्णाङ्क src_विस्तारः, अचिह्नित पूर्णाङ्क src_height, पूर्णाङ्क dest_x, पूर्णाङ्क dest_y, अचिह्नित पूर्णाङ्क dest_विस्तारः, अचिह्नित पूर्णाङ्क dest_height);
प्रकारनिर्देश VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, पूर्णाङ्क x, पूर्णाङ्क y, अचिह्नित पूर्णाङ्क विस्तारः, अचिह्नित पूर्णाङ्क height, VAImageID image);

स्थिर FN_vaDeriveImage fn_vaDeriveImage = रिक्त;
स्थिर FN_vaDestroyImage fn_vaDestroyImage = रिक्त;
स्थिर FN_va_Map_बफर fn_va_Map_बफर = रिक्त;
स्थिर FN_vaSyncSurface fn_vaSyncSurface = रिक्त;
स्थिर FN_va_Unmap_बफर fn_va_Unmap_बफर = रिक्त;
स्थिर FN_va_Max_अङ्कः_Image_Formats fn_va_Max_अङ्कः_Image_Formats = रिक्त;
स्थिर FN_vaQueryImageFormats fn_vaQueryImageFormats = रिक्त;
स्थिर FN_vaCreateImage fn_vaCreateImage = रिक्त;
स्थिर FN_vaPutImage fn_vaPutImage = रिक्त;
स्थिर FN_vaGetImage fn_vaGetImage = रिक्त;

#परिभाषय vaDeriveImage fn_vaDeriveImage
#परिभाषय vaDestroyImage fn_vaDestroyImage
#परिभाषय va_Map_बफर fn_va_Map_बफर
#परिभाषय vaSyncSurface fn_vaSyncSurface
#परिभाषय va_Unmap_बफर fn_va_Unmap_बफर
#परिभाषय va_Max_अङ्कः_Image_Formats fn_va_Max_अङ्कः_Image_Formats
#परिभाषय vaQueryImageFormats fn_vaQueryImageFormats
#परिभाषय vaCreateImage fn_vaCreateImage
#परिभाषय vaPutImage fn_vaPutImage
#परिभाषय vaGetImage fn_vaGetImage


स्थिर std::shared_ptr<cv::plugin::impl::DynamicLib> आरोपय_Lib_VA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    ध्रुव std::string env_मार्गः = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    यदि (!env_मार्गः.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(env_मार्गः);
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
    स्थिर स्वचालित library = आरोपय_Lib_VA();
    यदि (!initialized)
    {
        यदि (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        स्वचालित& lib = *library.get();
#परिभाषय VA_आरोपय_चिह्नम्(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_चिह्नम्(#name)); \
        यदि (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = सत्य; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_आरोपय_चिह्नम्(vaDeriveImage);
        VA_आरोपय_चिह्नम्(vaDestroyImage);
        VA_आरोपय_चिह्नम्(va_Map_बफर);
        VA_आरोपय_चिह्नम्(vaSyncSurface);
        VA_आरोपय_चिह्नम्(va_Unmap_बफर);
        VA_आरोपय_चिह्नम्(va_Max_अङ्कः_Image_Formats);
        VA_आरोपय_चिह्नम्(vaQueryImageFormats);
        VA_आरोपय_चिह्नम्(vaCreateImage);
        VA_आरोपय_चिह्नम्(vaPutImage);
        VA_आरोपय_चिह्नम्(vaGetImage);
        initialized = सत्य;
    }
    यदि (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
