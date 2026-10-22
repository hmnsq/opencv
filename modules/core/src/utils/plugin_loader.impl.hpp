// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#অন্তর্ভাব "opencv2/core/utils/plugin_loader.private.hpp"

#যদি !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#যদি 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#অন্যথা
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#অন্তর্ভাব <opencv2/core/utils/logger.hpp>
#endif

নামাকাশ cv { নামাকাশ plugin { নামাকাশ impl {

DynamicLib::DynamicLib(ধ্রুব FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(অসত্য)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    যদি (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    অন্যথা যদি (handle)
    {
        CV_LOG_INFO(রিক্ত, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

শূন্য* DynamicLib::get_প্রতীক(ধ্রুব বর্ণ* প্রতীক_Name) ধ্রুব
{
    যদি (!handle)
    {
        প্রত্যাগম 0;
    }
    শূন্য* res = get_প্রতীক_2(handle, প্রতীক_Name);
    যদি (!res)
    {
        CV_LOG_DEBUG(রিক্ত, "No symbol '" << প্রতীক_Name << "' in " << toPrintablePath(fname));
    }
    প্রত্যাগম res;
}

std::string DynamicLib::getName() ধ্রুব
{
    প্রত্যাগম toPrintablePath(fname);
}

শূন্য DynamicLib::libraryLoad(ধ্রুব FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(রিক্ত, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

শূন্য DynamicLib::libraryRelease()
{
    যদি (handle)
    {
        CV_LOG_INFO(রিক্ত, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
