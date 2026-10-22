// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ಅನ್ತರ್ಭಾವ "opencv2/core/utils/plugin_loader.private.hpp"

#ಯದಿ !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#ಯದಿ 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#ಅನ್ಯಥಾ
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#ಅನ್ತರ್ಭಾವ <opencv2/core/utils/logger.hpp>
#endif

ನಾಮಾಕಾಶ cv { ನಾಮಾಕಾಶ plugin { ನಾಮಾಕಾಶ impl {

DynamicLib::DynamicLib(ಧ್ರುವ FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(ಅಸತ್ಯ)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    ಯದಿ (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    ಅನ್ಯಥಾ ಯದಿ (handle)
    {
        CV_LOG_INFO(ರಿಕ್ತ, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

ಶೂನ್ಯ* DynamicLib::get_ಚಿಹ್ನೆ(ಧ್ರುವ ವರ್ಣ* ಚಿಹ್ನೆ_Name) ಧ್ರುವ
{
    ಯದಿ (!handle)
    {
        ಪ್ರತ್ಯಾಗಮ 0;
    }
    ಶೂನ್ಯ* res = get_ಚಿಹ್ನೆ_2(handle, ಚಿಹ್ನೆ_Name);
    ಯದಿ (!res)
    {
        CV_LOG_DEBUG(ರಿಕ್ತ, "No symbol '" << ಚಿಹ್ನೆ_Name << "' in " << toPrintablePath(fname));
    }
    ಪ್ರತ್ಯಾಗಮ res;
}

std::string DynamicLib::getName() ಧ್ರುವ
{
    ಪ್ರತ್ಯಾಗಮ toPrintablePath(fname);
}

ಶೂನ್ಯ DynamicLib::libraryLoad(ಧ್ರುವ FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(ರಿಕ್ತ, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

ಶೂನ್ಯ DynamicLib::libraryRelease()
{
    ಯದಿ (handle)
    {
        CV_LOG_INFO(ರಿಕ್ತ, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
