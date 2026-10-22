// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#অন্তর্ভাৱ "opencv2/core/utils/plugin_loader.private.hpp"

#যদি !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#যদি 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#অন্যথা
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#অন্তর্ভাৱ <opencv2/core/utils/logger.hpp>
#endif

নামাকাশ cv { নামাকাশ plugin { নামাকাশ impl {

DynamicLib::DynamicLib(ধ্রুৱ FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(মিছা)
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

শূন্য* DynamicLib::get_প্ৰতীক(ধ্রুৱ ৱর্ণ* প্ৰতীক_Name) ধ্রুৱ
{
    যদি (!handle)
    {
        ঘূৰাই 0;
    }
    শূন্য* res = get_প্ৰতীক_2(handle, প্ৰতীক_Name);
    যদি (!res)
    {
        CV_LOG_DEBUG(রিক্ত, "No symbol '" << প্ৰতীক_Name << "' in " << toPrintablePath(fname));
    }
    ঘূৰাই res;
}

std::string DynamicLib::getName() ধ্রুৱ
{
    ঘূৰাই toPrintablePath(fname);
}

শূন্য DynamicLib::libraryLoad(ধ্রুৱ FileSystemPath_t& filename)
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
