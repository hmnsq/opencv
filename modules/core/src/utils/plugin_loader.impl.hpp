// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ଅନ୍ତର୍ଭାଵ "opencv2/core/utils/plugin_loader.private.hpp"

#ଯଦି !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#ଯଦି 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ଅନ୍ୟଥା
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#ଅନ୍ତର୍ଭାଵ <opencv2/core/utils/logger.hpp>
#endif

ନାମାକାଶ cv { ନାମାକାଶ plugin { ନାମାକାଶ impl {

DynamicLib::DynamicLib(ଅଚଳ FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(ମିଥ୍ୟା)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    ଯଦି (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    ଅନ୍ୟଥା ଯଦି (handle)
    {
        CV_LOG_INFO(ଖାଲି, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

ଶୂନ୍ୟ* DynamicLib::get_ପ୍ରତୀକ(ଅଚଳ ଅକ୍ଷର* ପ୍ରତୀକ_Name) ଅଚଳ
{
    ଯଦି (!handle)
    {
        ଫେରନ୍ତୁ 0;
    }
    ଶୂନ୍ୟ* res = get_ପ୍ରତୀକ_2(handle, ପ୍ରତୀକ_Name);
    ଯଦି (!res)
    {
        CV_LOG_DEBUG(ଖାଲି, "No symbol '" << ପ୍ରତୀକ_Name << "' in " << toPrintablePath(fname));
    }
    ଫେରନ୍ତୁ res;
}

std::string DynamicLib::getName() ଅଚଳ
{
    ଫେରନ୍ତୁ toPrintablePath(fname);
}

ଶୂନ୍ୟ DynamicLib::libraryLoad(ଅଚଳ FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(ଖାଲି, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

ଶୂନ୍ୟ DynamicLib::libraryRelease()
{
    ଯଦି (handle)
    {
        CV_LOG_INFO(ଖାଲି, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
