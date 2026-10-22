// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#અન્તર્ભાવ "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#અન્તર્ભાવ "opencv2/core/utils/configuration.private.hpp"

નામાકાશ cv { નામાકાશ detail {

પ્રકારવ્યાખ્યા VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, શૂન્ય **pbuf);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
પ્રકારવ્યાખ્યા પૂર્ણાંક (*FN_va_Max_અંક_Image_Formats)(VADisplay dpy);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, પૂર્ણાંક *અંક_formats);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, પૂર્ણાંક width, પૂર્ણાંક height, VAImage *image);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, પૂર્ણાંક src_x, પૂર્ણાંક src_y, unsigned પૂર્ણાંક src_width, unsigned પૂર્ણાંક src_height, પૂર્ણાંક dest_x, પૂર્ણાંક dest_y, unsigned પૂર્ણાંક dest_width, unsigned પૂર્ણાંક dest_height);
પ્રકારવ્યાખ્યા VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, પૂર્ણાંક x, પૂર્ણાંક y, unsigned પૂર્ણાંક width, unsigned પૂર્ણાંક height, VAImageID image);

સ્થિર FN_vaDeriveImage fn_vaDeriveImage = ખાલી;
સ્થિર FN_vaDestroyImage fn_vaDestroyImage = ખાલી;
સ્થિર FN_vaMapBuffer fn_vaMapBuffer = ખાલી;
સ્થિર FN_vaSyncSurface fn_vaSyncSurface = ખાલી;
સ્થિર FN_vaUnmapBuffer fn_vaUnmapBuffer = ખાલી;
સ્થિર FN_va_Max_અંક_Image_Formats fn_va_Max_અંક_Image_Formats = ખાલી;
સ્થિર FN_vaQueryImageFormats fn_vaQueryImageFormats = ખાલી;
સ્થિર FN_vaCreateImage fn_vaCreateImage = ખાલી;
સ્થિર FN_vaPutImage fn_vaPutImage = ખાલી;
સ્થિર FN_vaGetImage fn_vaGetImage = ખાલી;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define va_Max_અંક_Image_Formats fn_va_Max_અંક_Image_Formats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


સ્થિર std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    અચલ std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    જો (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        પાછા lib;
    }
    સ્થિર અચલ અક્ષર* અચલ candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    માટે (પૂર્ણાંક i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        જો (lib->isLoaded())
            તોડો;
    }
    પાછા lib;
}
સ્થિર શૂન્ય init_libva()
{
    સ્થિર તાર્કિક initialized = ખોટું;
    સ્થિર auto library = loadLibVA();
    જો (!initialized)
    {
        જો (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_પ્રતીક(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.get_પ્રતીક(#name)); \
        જો (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = સાચું; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_પ્રતીક(vaDeriveImage);
        VA_LOAD_પ્રતીક(vaDestroyImage);
        VA_LOAD_પ્રતીક(vaMapBuffer);
        VA_LOAD_પ્રતીક(vaSyncSurface);
        VA_LOAD_પ્રતીક(vaUnmapBuffer);
        VA_LOAD_પ્રતીક(va_Max_અંક_Image_Formats);
        VA_LOAD_પ્રતીક(vaQueryImageFormats);
        VA_LOAD_પ્રતીક(vaCreateImage);
        VA_LOAD_પ્રતીક(vaPutImage);
        VA_LOAD_પ્રતીક(vaGetImage);
        initialized = સાચું;
    }
    જો (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
