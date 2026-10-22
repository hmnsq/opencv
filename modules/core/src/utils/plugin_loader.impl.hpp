// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

//
// Not a standalone header, part of filesystem.cpp
//

#ضمن "opencv2/core/utils/plugin_loader.private.hpp"

#𐡄𐡍 !OPENCV_HAVE_FILESYSTEM_SUPPORT
#error "Invalid build configuration"
#endif

#𐡄𐡍 0  // TODO
#ifdef NDEBUG
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_DEBUG + 1
#وإلا
#define CV_LOG_STRIP_LEVEL CV_LOG_LEVEL_VERBOSE + 1
#endif
#ضمن <opencv2/core/utils/logger.hpp>
#endif

نطاق cv { نطاق plugin { نطاق impl {

DynamicLib::DynamicLib(ثابت FileSystemPath_t& filename)
    : handle(0), fname(filename), disableAutoUnloading_(خطأ)
{
    libraryLoad(filename);
}

DynamicLib::~DynamicLib()
{
    𐡄𐡍 (!disableAutoUnloading_)
    {
        libraryRelease();
    }
    وإلا 𐡄𐡍 (handle)
    {
        CV_LOG_INFO(عدم, "skip auto unloading (disabled): " << toPrintablePath(fname));
        handle = 0;
    }
}

فراغ* DynamicLib::getSymbol(ثابت حرف* symbolName) ثابت
{
    𐡄𐡍 (!handle)
    {
        أرجع 0;
    }
    فراغ* res = getSymbol_(handle, symbolName);
    𐡄𐡍 (!res)
    {
        CV_LOG_DEBUG(عدم, "No symbol '" << symbolName << "' in " << toPrintablePath(fname));
    }
    أرجع res;
}

std::string DynamicLib::getName() ثابت
{
    أرجع toPrintablePath(fname);
}

فراغ DynamicLib::libraryLoad(ثابت FileSystemPath_t& filename)
{
    handle = libraryLoad_(filename);
    CV_LOG_INFO(عدم, "load " << toPrintablePath(filename) << " => " << (handle ? "OK" : "FAILED"));
}

فراغ DynamicLib::libraryRelease()
{
    𐡄𐡍 (handle)
    {
        CV_LOG_INFO(عدم, "unload "<< toPrintablePath(fname));
        libraryRelease_(handle);
        handle = 0;
    }
}

}}}  // namespace
