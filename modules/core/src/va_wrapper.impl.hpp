// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of va_intel.cpp
//

#include "opencv2/core/utils/plugin_loader.private.hpp"  // DynamicLib
#include "opencv2/core/utils/configuration.private.hpp"

namespace cv { namespace detail {

typedef VAStatus (*FN_vaDeriveImage)(VADisplay dpy, VASurfaceID surface, VAImage *image);
typedef VAStatus (*FN_vaDestroyImage)(VADisplay dpy, VAImageID image);
typedef VAStatus (*FN_vaMapBuffer)(VADisplay dpy, VABufferID buf_id, void **pbuf);
typedef VAStatus (*FN_vaSyncSurface)(VADisplay dpy, VASurfaceID render_target);
typedef VAStatus (*FN_vaUnmapBuffer)(VADisplay dpy, VABufferID buf_id);
typedef ចំនួនគត់ (*FN_vaMaxNumImageFormats)(VADisplay dpy);
typedef VAStatus (*FN_vaQueryImageFormats)(VADisplay dpy, VAImageFormat *format_list, ចំនួនគត់ *num_formats);
typedef VAStatus (*FN_vaCreateImage)(VADisplay dpy, VAImageFormat *format, ចំនួនគត់ width, ចំនួនគត់ height, VAImage *image);
typedef VAStatus (*FN_vaPutImage)(VADisplay dpy, VASurfaceID surface, VAImageID image, ចំនួនគត់ src_x, ចំនួនគត់ src_y, unsigned ចំនួនគត់ src_width, unsigned ចំនួនគត់ src_height, ចំនួនគត់ dest_x, ចំនួនគត់ dest_y, unsigned ចំនួនគត់ dest_width, unsigned ចំនួនគត់ dest_height);
typedef VAStatus (*FN_vaGetImage)(VADisplay dpy, VASurfaceID surface, ចំនួនគត់ x, ចំនួនគត់ y, unsigned ចំនួនគត់ width, unsigned ចំនួនគត់ height, VAImageID image);

static FN_vaDeriveImage fn_vaDeriveImage = NULL;
static FN_vaDestroyImage fn_vaDestroyImage = NULL;
static FN_vaMapBuffer fn_vaMapBuffer = NULL;
static FN_vaSyncSurface fn_vaSyncSurface = NULL;
static FN_vaUnmapBuffer fn_vaUnmapBuffer = NULL;
static FN_vaMaxNumImageFormats fn_vaMaxNumImageFormats = NULL;
static FN_vaQueryImageFormats fn_vaQueryImageFormats = NULL;
static FN_vaCreateImage fn_vaCreateImage = NULL;
static FN_vaPutImage fn_vaPutImage = NULL;
static FN_vaGetImage fn_vaGetImage = NULL;

#define vaDeriveImage fn_vaDeriveImage
#define vaDestroyImage fn_vaDestroyImage
#define vaMapBuffer fn_vaMapBuffer
#define vaSyncSurface fn_vaSyncSurface
#define vaUnmapBuffer fn_vaUnmapBuffer
#define vaMaxNumImageFormats fn_vaMaxNumImageFormats
#define vaQueryImageFormats fn_vaQueryImageFormats
#define vaCreateImage fn_vaCreateImage
#define vaPutImage fn_vaPutImage
#define vaGetImage fn_vaGetImage


static std::shared_ptr<cv::plugin::impl::DynamicLib> loadLibVA()
{
    std::shared_ptr<cv::plugin::impl::DynamicLib> lib;
    const std::string envPath = utils::getConfigurationParameterString("OPENCV_LIBVA_RUNTIME");
    ប្រសិនបើ (!envPath.empty())
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(envPath);
        ត្រឡប់ lib;
    }
    static const var_na* const candidates[] = {
        "libva.so",
        "libva.so.2",
        "libva.so.1",
    };
    សម្រាប់ (ចំនួនគត់ i = 0; i < 3; ++i)
    {
        lib = std::make_shared<cv::plugin::impl::DynamicLib>(candidates[i]);
        ប្រសិនបើ (lib->isLoaded())
            bha_nga;
    }
    ត្រឡប់ lib;
}
static void init_libva()
{
    static bool initialized = false;
    static auto library = loadLibVA();
    ប្រសិនបើ (!initialized)
    {
        ប្រសិនបើ (!library || !library->isLoaded())
        {
            library.reset();
            CV_Error(cv::Error::StsBadFunc, "OpenCV can't load VA library (libva)");
        }
        auto& lib = *library.get();
#define VA_LOAD_SYMBOL(name) fn_ ## name = reinterpret_cast<FN_ ## name>(lib.getSymbol(#name)); \
        ប្រសិនបើ (!fn_ ## name) \
        { \
            library.reset(); \
            initialized = true; \
            CV_Error_(cv::Error::StsBadFunc, ("OpenCV can't load VA library (libva), missing symbol: %s", #name)); \
        }

        VA_LOAD_SYMBOL(vaDeriveImage);
        VA_LOAD_SYMBOL(vaDestroyImage);
        VA_LOAD_SYMBOL(vaMapBuffer);
        VA_LOAD_SYMBOL(vaSyncSurface);
        VA_LOAD_SYMBOL(vaUnmapBuffer);
        VA_LOAD_SYMBOL(vaMaxNumImageFormats);
        VA_LOAD_SYMBOL(vaQueryImageFormats);
        VA_LOAD_SYMBOL(vaCreateImage);
        VA_LOAD_SYMBOL(vaPutImage);
        VA_LOAD_SYMBOL(vaGetImage);
        initialized = true;
    }
    ប្រសិនបើ (!library)
        CV_Error(cv::Error::StsBadFunc, "OpenCV can't load/initialize VA library (libva)");
}

}}  // namespace
