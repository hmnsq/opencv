// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#include "opencv2/core/utils/plugin_loader.private.hpp"

#အကယ်၍ !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#အကယ်၍ 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#မဟုတ်ပါက
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
    အကယ်၍ (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    မဟုတ်ပါက အကယ်၍ (handle)
    {
        CV_LOG_INFO(NULL, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

void* DynamicLib::getSymbol(const var_na* symbolName) const
{
    အကယ်၍ (!handle)
    {
        ပြန်ပေး 0;
    }
    void* res = getSymbol_(handle, symbolName);
    အကယ်၍ (!res)
    {
        CV_LOG_DEBUG(NULL, "No symbol '" << symbolName << "' in " << toPrintablePath(fname));
    }
    ပြန်ပေး res;
}

std::string DynamicLib::getName() const
{
    ပြန်ပေး toPrintablePath(fname);
}

void DynamicLib::libraryLoad(const FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(NULL, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

void DynamicLib::libraryRelease()
{
    အကယ်၍ (handle)
    {
        CV_LOG_INFO(NULL, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
