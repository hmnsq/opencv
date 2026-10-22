// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#شامل "opencv2/core/utils/plugin_loader.private.hpp"

#اگر !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#اگر 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وگرنه
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#شامل <opencv2/core/utils/logger.hpp>
#endif

فضاینام cv { فضاینام plugin { فضاینام impl {

DynamicLib::DynamicLib(ثابت FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(نادرست)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    اگر (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    وگرنه اگر (handle)
    {
        CV_LOG_INFO(پوچ, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

تهی* DynamicLib::get_نماد(ثابت نویسه* نماد_Name) ثابت
{
    اگر (!handle)
    {
        بازگشت 0;
    }
    تهی* res = get_نماد_2(handle, نماد_Name);
    اگر (!res)
    {
        CV_LOG_DEBUG(پوچ, "No symbol '" << نماد_Name << "' in " << toPrintablePath(fname));
    }
    بازگشت res;
}

std::string DynamicLib::getName() ثابت
{
    بازگشت toPrintablePath(fname);
}

تهی DynamicLib::libraryLoad(ثابت FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(پوچ, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

تهی DynamicLib::libraryRelease()
{
    اگر (handle)
    {
        CV_LOG_INFO(پوچ, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
