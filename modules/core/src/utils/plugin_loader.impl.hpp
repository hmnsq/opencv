// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#include "opencv2/core/utils/plugin_loader.private.hpp"

#ᮚᮦᮔ᮪ !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#ᮚᮦᮔ᮪ 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ᮜᮤᮚ
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#include <opencv2/core/utils/logger.hpp>
#endif

namespace cv { namespace plugin { namespace impl {

DynamicLib::DynamicLib(const FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(false)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    ᮚᮦᮔ᮪ (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    ᮜᮤᮚ ᮚᮦᮔ᮪ (handle)
    {
        CV_LOG_INFO(NULL, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

void* DynamicLib::getSymbol(const var_na* symbolName) const
{
    ᮚᮦᮔ᮪ (!handle)
    {
        ᮝᮜᮤ 0;
    }
    void* res = getSymbol_(handle, symbolName);
    ᮚᮦᮔ᮪ (!res)
    {
        CV_LOG_DEBUG(NULL, "No symbol '" << symbolName << "' in " << toPrintablePath(fname));
    }
    ᮝᮜᮤ res;
}

std::string DynamicLib::getName() const
{
    ᮝᮜᮤ toPrintablePath(fname);
}

void DynamicLib::libraryLoad(const FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(NULL, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

void DynamicLib::libraryRelease()
{
    ᮚᮦᮔ᮪ (handle)
    {
        CV_LOG_INFO(NULL, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
