// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#समावेश "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#समावेश "opencv2/core/utils/configuration.private.hpp"

namespace cv { namespace detail {

प्रकार VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
प्रकार VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
प्रकार VAStatus (*FN_va_Map_बफर)(VADisplay dpy, VABufferID buf_id, व्योम **pbuf);
प्रकार VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
प्रकार VAStatus (*FN_va_Unmap_बफर)(VADisplay dpy, VABufferID buf_id);
प्रकार पूर्णांक (*FN_va_Max_अंक_Image_Formats)(VADisplay dpy);
प्रकार VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, पूर्णांक *अंक_formats);
प्रकार VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, पूर्णांक width, पूर्णांक height, VAImage *image);
प्रकार VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, पूर्णांक src_x, पूर्णांक src_y, अचिन्हित पूर्णांक src_width, अचिन्हित पूर्णांक src_height, पूर्णांक dest_x, पूर्णांक dest_y, अचिन्हित पूर्णांक dest_width, अचिन्हित पूर्णांक dest_height);
प्रकार VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, पूर्णांक x, पूर्णांक y, अचिन्हित पूर्णांक width, अचिन्हित पूर्णांक height, VAImageID image);

अटल FN_vaDeriveImage fn_vaDeriveImage = शून्य;
अटल FN_vaDestroyImage fn_vaDestroyImage = शून्य;
अटल FN_va_Map_बफर fn_va_Map_बफर = शून्य;
अटल FN_vaSyncSurface fn_vaSyncSurface = शून्य;
अटल FN_va_Unmap_बफर fn_va_Unmap_बफर = शून्य;
अटल FN_va_Max_अंक_Image_Formats fn_va_Max_अंक_Image_Formats = शून्य;
अटल FN_vaQueryImageFormats fn_vaQueryImageFormats = शून्य;
अटल FN_vaCreateImage fn_vaCreateImage = शून्य;
अटल FN_vaPutImage fn_vaPutImage = शून्य;
अटल FN_vaGetImage fn_vaGetImage = शून्य;

#घोषणा vaDeriveImage fn_vaDeriveImage
#घोषणा vaDestroyImage fn_vaDestroyImage
#घोषणा va_Map_बफर fn_va_Map_बफर
#घोषणा vaSyncSurface fn_vaSyncSurface
#घोषणा va_Unmap_बफर fn_va_Unmap_बफर
#घोषणा va_Max_अंक_Image_Formats fn_va_Max_अंक_Image_Formats
#घोषणा vaQueryImageFormats fn_vaQueryImageFormats
#घोषणा vaCreateImage fn_vaCreateImage
#घोषणा vaPutImage fn_vaPutImage
#घोषणा vaGetImage fn_vaGetImage


अटल std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    स्थिर std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    अगर (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        वापस lib;
    }
    अटल स्थिर अक्षर* स्थिर candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    क्रम (पूर्णांक i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        अगर (lib->isLoaded())
            अवरोध;
    }
    वापस lib;
}
अटल व्योम init_libva()
{
    अटल bool initialized = false;
    अटल स्वतः library = loadLibVA();
    अगर (!initialized)
    {
        अगर (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        स्वतः& lib = *library.लो();
#घोषणा VA_LOAD_प्रतीक(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_प्रतीक(#name)); \
        अगर (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = true; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_प्रतीक(vaDeriveImage);
        VA_LOAD_प्रतीक(vaDestroyImage);
        VA_LOAD_प्रतीक(va_Map_बफर);
        VA_LOAD_प्रतीक(vaSyncSurface);
        VA_LOAD_प्रतीक(va_Unmap_बफर);
        VA_LOAD_प्रतीक(va_Max_अंक_Image_Formats);
        VA_LOAD_प्रतीक(vaQueryImageFormats);
        VA_LOAD_प्रतीक(vaCreateImage);
        VA_LOAD_प्रतीक(vaPutImage);
        VA_LOAD_प्रतीक(vaGetImage);
        initialized = true;
    }
    अगर (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
